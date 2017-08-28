#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <limits>

namespace utils {

/** Enumeration class for axis */
enum class Axis {
  X,
  Y,
  Z
};

/** Creates rotation matrix
 *  @param  angle  The rotation angle
 *  @param  axis   The rotation axis
 *  @return  The rotation matrix
 */
glm::mat4 rotation_matrix(float angle, Axis axis) {
  glm::mat4 matrix(1.0);  // identity matrix
  float rangle = glm::radians(angle);
  // We want to unique situation when in matrix are -0.0f, so we perform
  // additional checking
  float sin_a = glm::sin(rangle);
  if (fabs(sin_a) < std::numeric_limits<float>::epsilon())
    sin_a = 0.0f;
  float cos_a = glm::cos(rangle);
  if (fabs(cos_a) < std::numeric_limits<float>::epsilon())
    cos_a = 0.0f;
  float msin_a = fabs(sin_a) < std::numeric_limits<float>::epsilon() ?
      0.0f : (-1.0f) * sin_a;

  if (axis == Axis::X) {
    ((float*)glm::value_ptr(matrix))[5] = cos_a;
    ((float*)glm::value_ptr(matrix))[6] = sin_a;
    ((float*)glm::value_ptr(matrix))[9] = msin_a;
    ((float*)glm::value_ptr(matrix))[10] = cos_a;
  } else if (axis == Axis::Y) {
    ((float*)glm::value_ptr(matrix))[0] = cos_a;
    ((float*)glm::value_ptr(matrix))[2] = msin_a;
    ((float*)glm::value_ptr(matrix))[8] = sin_a;
    ((float*)glm::value_ptr(matrix))[10] = cos_a;
  } else {
    ((float*)glm::value_ptr(matrix))[0] = cos_a;
    ((float*)glm::value_ptr(matrix))[1] = sin_a;
    ((float*)glm::value_ptr(matrix))[4] = msin_a;
    ((float*)glm::value_ptr(matrix))[5] = cos_a;
  }

  return matrix;
}

/** Rotates matrix
 *  @param  matrix  The matrix to be rotated
 *  @param  angle   The rotation angle
 *  @param  axis    The rotation axis
 *  @return  The rotation matrix
 */
glm::mat4 rotate(glm::mat4 matrix, float angle, Axis axis) {
  return matrix * rotation_matrix(angle, axis);
}

/** Translates matrix
 *  @param  matrix  The matrix to be rotated
 *  @param  translation   The translation vector
 *  @return  The translated matrix
 */
glm::mat4 translate(glm::mat4 matrix, glm::vec3 translation) {
  ((float*)glm::value_ptr(matrix))[12] += translation.x;
  ((float*)glm::value_ptr(matrix))[13] += translation.y;
  ((float*)glm::value_ptr(matrix))[14] += translation.z;
  return matrix;
}

/** Converts matrix to string
 *  @param  matrix  The matrix to be converted
 *  @return  The created string
 */
std::string mat4tos(const glm::mat4& matrix) {
  std::string result;
  for (int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++)
      result += std::to_string(((float*)glm::value_ptr(matrix))[4*j+i]) + ", ";

    result += "\n";
  }
  return result;
}

/** Converts vector to string
 *  @param  vector  The vector to be converted
 *  @return  The created string
 */
std::string vec3tos(const glm::vec3 &vector)
{
  std::string result;
  for (int i = 0; i < 3; i++) {
    result += std::to_string(((float *)glm::value_ptr(vector))[i]);
    if (i != 2)
      result += ", ";
  }

  result += "\n";

  return result;
}

} // namespace
#endif // UTILS_H