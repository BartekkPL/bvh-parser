#ifndef BVH_H
#define BVH_H

#include "joint.h"

#include <memory>
#include <vector>

namespace bvh {

class Bvh {
  public:
    void add_joint(const std::shared_ptr <Joint> joint) {
      joints_.push_back(joint);
    }
    const Joint* root_joint() const { return root_joint_.get(); }
    const std::vector <std::shared_ptr <Joint>> joints() const {
      return joints_;
    }
    unsigned num_frames() const { return num_frames_; }
    double frame_time() const { return frame_time_; }
    void set_root_joint(const std::shared_ptr<Joint> arg) { root_joint_ = arg; }
    void set_joints(const std::vector <std::shared_ptr <Joint>> arg) {
      joints_ = arg;
    }
    void set_num_frames(const unsigned arg) { num_frames_ = arg; }
    void set_frame_time(const double arg) {frame_time_ = arg; }

  private:
    /** A root joint in this bvh file */
    std::shared_ptr<Joint> root_joint_;
    /** All joints in file in order of parse */
    std::vector <std::shared_ptr <Joint>> joints_;
    /** A number of motion frames in this bvh file */
    unsigned num_frames_;
    /** A time of single frame */
    double frame_time_;
};

} // namespace
#endif  // BVH_H
