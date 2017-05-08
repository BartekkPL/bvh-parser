#include "bvh.h"
#include <glm/gtc/matrix_transform.hpp>

namespace bvh {

void Bvh::recalculate_joints_ltm(std::shared_ptr<Joint> start_joint) {
  if (start_joint == NULL) {
    if (root_joint_ == NULL)
      return;
    else
      start_joint = root_joint_;
  }

  glm::mat4 origin_matrix = glm::translate(glm::mat4(1.0),
        glm::vec3(start_joint->offset().x, start_joint->offset().y,
        start_joint->offset().z));

  std::vector <std::vector <float>> data = start_joint->channel_data();

  for (int i = 0; i < num_frames_; i++) {
    glm::mat4 matrix = origin_matrix;

    for (int j = start_joint->channels_order().size() - 1; j >= 0; j--) {
      if (start_joint->channels_order()[j] == Joint::Channel::XPOSITION)
        matrix = glm::translate(matrix, glm::vec3(data[i][j], 0, 0));
      else if (start_joint->channels_order()[j] == Joint::Channel::YPOSITION)
        matrix = glm::translate(matrix, glm::vec3(0, data[i][j], 0));
      else if (start_joint->channels_order()[j] == Joint::Channel::ZPOSITION)
        matrix = glm::translate(matrix, glm::vec3(0, 0, data[i][j]));
      else if (start_joint->channels_order()[j] == Joint::Channel::XROTATION)
        matrix = glm::rotate(matrix, data[i][j], glm::vec3(1, 0, 0));
      else if (start_joint->channels_order()[j] == Joint::Channel::YROTATION)
        matrix = glm::rotate(matrix, data[i][j], glm::vec3(0, 1, 0));
      else if (start_joint->channels_order()[j] == Joint::Channel::ZROTATION)
        matrix = glm::rotate(matrix, data[i][j], glm::vec3(0, 0, 1));
    }

    if (start_joint->parent() != NULL)
      matrix = start_joint->parent()->matrix(i) * matrix;

    start_joint->set_matrix(matrix, i);
  }

  for (auto& child : start_joint->children()) {
    recalculate_joints_ltm(child);
  }
}

}