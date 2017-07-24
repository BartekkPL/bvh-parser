#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace utils {

enum class Axis {
  X,
  Y,
  Z
};

glm::mat4 rotation_matrix(float angle, Axis axis) {
  glm::mat4 matrix(1.0);
  float sin_a = glm::sin(glm::radians(angle));
  float cos_a = glm::cos(glm::radians(angle));

  if (axis == Axis::X) {
    ((float*)glm::value_ptr(matrix))[5] = cos_a;
    ((float*)glm::value_ptr(matrix))[6] = sin_a;
    ((float*)glm::value_ptr(matrix))[9] = (-1) * sin_a;
    ((float*)glm::value_ptr(matrix))[10] = cos_a;
  } else if (axis == Axis::Y) {
    ((float*)glm::value_ptr(matrix))[0] = cos_a;
    ((float*)glm::value_ptr(matrix))[2] = (-1) * sin_a;
    ((float*)glm::value_ptr(matrix))[8] = sin_a;
    ((float*)glm::value_ptr(matrix))[10] = cos_a;
  } else {
    ((float*)glm::value_ptr(matrix))[0] = cos_a;
    ((float*)glm::value_ptr(matrix))[1] = sin_a;
    ((float*)glm::value_ptr(matrix))[4] = (-1) * sin_a;
    ((float*)glm::value_ptr(matrix))[5] = cos_a;
  }

  return matrix;
}

glm::mat4 rotate(glm::mat4 matrix, float angle, Axis axis) {
  return matrix * rotation_matrix(angle, axis);
}

std::string mat4tos(const glm::mat4& matrix) {
  std::string result;
  for (int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++)
      result += std::to_string(((float*)glm::value_ptr(matrix))[4*j+i]) + ", ";

    result += "\n";
  }
  return result;
}

std::string vec4tos(const glm::vec4 &vector)
{
  std::string result;
  for (int i = 0; i < 4; i++)
    result += std::to_string(((float *)glm::value_ptr(vector))[i]) + "\t";

  result += "\n";

  return result;
}

} // namespace
#endif // UTILS_H