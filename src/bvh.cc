#include "bvh.h"

#include "easylogging++.h"
#include "utils.h"

#include <glm/gtc/matrix_transform.hpp>

namespace bvh {

void Bvh::recalculate_joints_ltm(std::shared_ptr<Joint> start_joint) {

      if (start_joint == NULL)
  {
    if (root_joint_ == NULL)
      return;
    else
      start_joint = root_joint_;
  }

  LOG(DEBUG) << "recalculate_joints_ltm: " << start_joint->name();

  glm::mat4 offmat_backup = glm::translate(glm::mat4(1.0),
        glm::vec3(start_joint->offset().x, start_joint->offset().y,
        start_joint->offset().z));

  std::vector<std::vector<float>> data = start_joint->channel_data();

  for (int i = 0; i < num_frames_; i++) {
    glm::mat4 offmat = offmat_backup; // offset matrix
    glm::mat4 rmat(1.0);  // identity matrix set on rotation matrix
    glm::mat4 tmat(1.0);  // identity matrix set on translation matrix

    for (int j = 0;  j < start_joint->channels_order().size(); j++) {
      if (start_joint->channels_order()[j] == Joint::Channel::XPOSITION)
        tmat = glm::translate(tmat, glm::vec3(data[i][j], 0, 0));
      else if (start_joint->channels_order()[j] == Joint::Channel::YPOSITION)
        tmat = glm::translate(tmat, glm::vec3(0, data[i][j], 0));
      else if (start_joint->channels_order()[j] == Joint::Channel::ZPOSITION)
        tmat = glm::translate(tmat, glm::vec3(0, 0, data[i][j]));
      else if (start_joint->channels_order()[j] == Joint::Channel::XROTATION)
        rmat = utils::rotate(rmat, data[i][j], utils::Axis::X);
      else if (start_joint->channels_order()[j] == Joint::Channel::YROTATION)
        rmat = utils::rotate(rmat, data[i][j], utils::Axis::Y);
      else if (start_joint->channels_order()[j] == Joint::Channel::ZROTATION)
        rmat = utils::rotate(rmat, data[i][j], utils::Axis::Z);
    }

    glm::mat4 ltm; // local transformation matrix

    if (start_joint->parent() != NULL)
      ltm = start_joint->parent()->ltm(i) * offmat;
    else
      ltm = tmat * offmat;

    start_joint->set_pos(ltm[3]);
    LOG(TRACE) << "Joint world position: " << utils::vec3tos(ltm[3]);

    ltm = ltm * rmat;

    LOG(TRACE) << "Local transformation matrix: \n" << utils::mat4tos(ltm);

    start_joint->set_ltm(ltm, i);
  }

  for (auto& child : start_joint->children()) {
    recalculate_joints_ltm(child);
  }
}

}
