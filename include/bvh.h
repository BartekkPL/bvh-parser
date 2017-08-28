#ifndef BVH_H
#define BVH_H

#include "joint.h"

#include <memory>
#include <vector>

namespace bvh {

/** Class created for storing motion data from bvh file */
class Bvh {
 public:
  /** Constructor of Bvh object
   *  @details  Initializes local variables
   */
  Bvh() : num_frames_(0), frame_time_(0), num_channels_(0) {}

  /**
   * Recalculation of local transformation matrix for each frame in each joint
   *
   * Should be called to set local_transformation_matrix vectors in joints
   * structures.
   *
   * @param start_joint  A joint of which each child local transformation
   * matrix will be recalculated, as default it is NULL which will be resolved
   * to root_joint in method body
   */
  void recalculate_joints_ltm(std::shared_ptr<Joint> start_joint = NULL);

  /** Adds joint to Bvh object
   *  @details  Adds joint and increases number of data channels
   *  @param  joint  The joint that will be added
   */
  void add_joint(const std::shared_ptr<Joint> joint) {
    joints_.push_back(joint);
    num_channels_ += joint->num_channels();
  }

  /** Gets the root joint
   *  @return  The root joint
   */
  const std::shared_ptr<Joint> root_joint() const { return root_joint_; }

  /** Gets all joints
   *  @return  The all joints
   */
  const std::vector <std::shared_ptr <Joint>> joints() const {
    return joints_;
  }

  /** Gets the number of data frames
   *  @return  The number of frames
   */
  unsigned num_frames() const { return num_frames_; }

  /** Gets the frame time
   *  @return  The single frame time (in second)
   */
  double frame_time() const { return frame_time_; }

  /** Gets the total number of channels
   *  @return  The number of data channels
   */
  unsigned num_channels() const { return num_channels_; }

  /** Sets the root joint
   *  @param  arg  The root joint to be set
   */
  void set_root_joint(const std::shared_ptr<Joint> arg) { root_joint_ = arg; }

  /** Sets the all joint at once
   *  @param  arg  The all joints to be set
   */
  void set_joints(const std::vector <std::shared_ptr <Joint>> arg) {
    joints_ = arg;
  }

  /** Sets the number of data frames
   *  @param  arg  The number of frames to be set
   */
  void set_num_frames(const unsigned arg) { num_frames_ = arg; }

  /** Sets the single data frame time
   *  @param  arg  The time of frame to be set
   */
  void set_frame_time(const double arg) { frame_time_ = arg; }

 private:
  /** A root joint in this bvh file */
  std::shared_ptr<Joint> root_joint_;
  /** All joints in file in order of parse */
  std::vector <std::shared_ptr <Joint>> joints_;
  /** A number of motion frames in this bvh file */
  unsigned num_frames_;
  /** A time of single frame */
  double frame_time_;
  /** Number of channels of all joints */
  unsigned num_channels_;
};

} // namespace
#endif  // BVH_H
