#include "joint.h"

#include <glm/gtc/matrix_transform.hpp>

namespace bvh {

void Joint::recalculate_position() {
  glm::mat4 position = glm::translate(glm::mat4(1.0), glm::vec3(offset_.x,
      offset_.y, offset_.z));

  for (int i = 0; i < channel_data_.size(); i++) {
    for (int j = 0; j < channels_order_.size(); j++) {
      if (channels_order_[j] == Channel::XPOSITION)
        position = glm::translate(position,
                                  glm::vec3(channel_data_[i][j], 0, 0));
      else if (channels_order_[j] == Channel::YPOSITION)
        position = glm::translate(position,
                                  glm::vec3(0, channel_data_[i][j], 0));
      else if (channels_order_[j] == Channel::ZPOSITION)
        position = glm::translate(position,
                                  glm::vec3(0, 0, channel_data_[i][j]));
      else if (channels_order_[j] == Channel::XROTATION)
        position = glm::rotate(position, channel_data_[i][j],
                               glm::vec3(1, 0, 0));
      else if (channels_order_[j] == Channel::YROTATION)
        position = glm::rotate(position, channel_data_[i][j],
                               glm::vec3(0, 1, 0));
      else if (channels_order_[j] == Channel::ZROTATION)
        position = glm::rotate(position, channel_data_[i][j],
                               glm::vec3(0, 0, 1));
    }
    joint_position_.push_back(position[3]);
  }
}

} // namespace
