#ifndef JOINT_H
#define JOINT_H

#include <memory>
#include <string>
#include <vector>

namespace bvh {

class Joint {
  public:
    /**
     *  A struct that keep offset of joint in relation to parent
     */
    struct Offset {
      float x;
      float y;
      float z;
    };

    /**
     * A enumeration type useful for set order of channels for every joint
     */
    enum class Channel {
      XPOSITION,
      YPOSITION,
      ZPOSITION,
      ZROTATION,
      XROTATION,
      YROTATION
    };

    /**
     * A string names for each channel
     */
    const std::vector<std::string> channel_name_str = {
      "XPOSITION",
      "YPOSITION",
      "ZPOSITION",
      "ZROTATION",
      "XROTATION",
      "YROTATION"
    };

    void add_frame_motion_data(const std::vector <float>& data) {
      channel_data_.push_back(data);
    }
    const std::shared_ptr <Joint> parent() const { return parent_; }
    const std::string name() const { return name_; }
    const Offset offset() const { return offset_; }
    const std::vector <Channel> channels_order() const {
      return channels_order_;
    }
    const std::vector <std::shared_ptr <Joint>> children() const { return children_; }
    const std::vector <std::vector <float>> channel_data() const {
      return channel_data_;
    }
    void set_parent(const std::shared_ptr <Joint> arg) { parent_ = arg; }
    void set_name(const std::string arg) { name_ = arg; }
    void set_offset(const Offset arg) { offset_ = arg; }
    void set_channels_order(const std::vector <Channel>& arg) {
      channels_order_ = arg;
    }
    void set_children(const std::vector <std::shared_ptr <Joint>>& arg) { children_ = arg; }
    void set_channel_data(const std::vector <std::vector <float>>& arg) {
      channel_data_ = arg;
    }

    const std::vector<std::string> get_channels_name() {
      std::vector<std::string> channel_names;

      for (int i = 0; i < channels_order_.size(); i++)
        channel_names.push_back(channel_name_str[static_cast<int>(channels_order_[i])]);

      return channel_names;
    }

  private:
    /** Parent joint in file hierarchy */
    std::shared_ptr <Joint> parent_;
    std::string name_;
    Offset offset_;
    /** Order of joint's input channels */
    std::vector <Channel> channels_order_;
    /** Pointers to joints that are children of this in hierarchy */
    std::vector <std::shared_ptr <Joint>> children_;
    /** Structure for keep joint's channel's data.
     *  Each vector keep data for one channel.
     */
    std::vector <std::vector <float> > channel_data_;
};

} // namespace
#endif // JOINT_H
