#include "bvh-parser.h"

#include "easylogging++.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <ios>
#include <string>

#define MULTI_HIERARCHY 0

namespace bf = boost::filesystem;

namespace {

const std::string kChannels = "CHANNELS";
const std::string kEnd = "End";
const std::string kEndSite = "End Site";
const std::string kFrame = "Frame";
const std::string kFrames = "Frames:";
const std::string kHierarchy = "HIERARCHY";
const std::string kJoint = "JOINT";
const std::string kMotion = "MOTION";
const std::string kOffset = "OFFSET";
const std::string kRoot = "ROOT";

const std::string kXpos = "Xposition";
const std::string kYpos = "Yposition";
const std::string kZpos = "Zposition";
const std::string kXrot = "Xrotation";
const std::string kYrot = "Yrotation";
const std::string kZrot = "Zrotation";

}

namespace bvh {

//##############################################################################
// Main parse function
//##############################################################################
int Bvh_parser::parse(const bf::path& path, Bvh* bvh) {
  LOG(INFO) << "Parsing file : " << path;

  path_ = path;
  bvh_ = bvh;

  bf::ifstream file;
  file.open(path_);

  if (file.is_open()) {
    std::string token;

#if MULTI_HIERARCHY == 1
    while (file.good()) {
#endif
      file >> token;
      if (token == kHierarchy) {
        int ret = parse_hierarchy(file);
        if (ret)
          return ret;
      } else {
        LOG(ERROR) << "Bad structure of .bvh file. " << kHierarchy
            << " should be on the top of the file";
        return -1;
      }
#if MULTI_HIERARCHY == 1
    }
#endif
  } else {
    LOG(ERROR) << "Cannot open file to parse : " << path_;
    return -1;
  }

  LOG(INFO) << "Successfully parsed file";
  return 0;
}

//##############################################################################
// Function parsing hierarchy
//##############################################################################
int Bvh_parser::parse_hierarchy(std::ifstream& file) {
  LOG(INFO) << "Parsing hierarchy";

  std::string token;
  int ret;

  if (file.good()) {
    file >> token;

    //##########################################################################
    // Parsing joints
    //##########################################################################
    if (token == kRoot) {
      std::shared_ptr <Joint> rootJoint;
      ret = parse_joint(file, nullptr, rootJoint);

      if (ret)
        return ret;

      bvh_->set_root_joint(rootJoint);
    } else {
      LOG(ERROR) << "Bad structure of .bvh file. Expected " << kRoot
          << ", but found \"" << token << "\"";
      return -1;
    }
  }

  if (file.good()) {
    file >> token;

    //##########################################################################
    // Parsing motion data
    //##########################################################################
    if (token == kMotion) {
      ret = parse_motion(file);

      if (ret)
        return ret;
    } else {
      LOG(ERROR) << "Bad structure of .bvh file. Expected " << kMotion
          << ", but found \"" << token << "\"";
      return -1;
    }
  }
  return 0;
}

//##############################################################################
// Function parsing joint
//##############################################################################
int Bvh_parser::parse_joint(std::ifstream& file,
    std::shared_ptr <Joint> parent, std::shared_ptr <Joint>& parsed) {

  LOG(TRACE) << "Parsing joint";

  std::shared_ptr<Joint> joint = std::make_shared<Joint>();
  joint->set_parent(parent);

  std::string name;
  file >> name;

  LOG(DEBUG) << "Joint name : " << name;

  joint->set_name(name);

  std::string token;
  std::vector <std::shared_ptr <Joint>> children;
  int ret;

  file >> token;  // Consuming '{'
  file >> token;

  //############################################################################
  // Offset parsing
  //############################################################################
  if (token == kOffset) {
    Joint::Offset offset;

    try {
      file >> offset.x >> offset.y >> offset.z;
    } catch (const std::ios_base::failure e) {
      LOG(ERROR) << "Failure while parsing offset";
      return -1;
    }

    joint->set_offset(offset);

    LOG(TRACE) << "Offset x: " << offset.x << ", y: " << offset.y << ", z: "
        << offset.z;

  } else {
    LOG(ERROR) << "Bad structure of .bvh file. Expected " << kOffset << ", but "
        << "found \"" << token << "\"";

    return -1;
  }

  file >> token;

  //############################################################################
  // Channels parsing
  //############################################################################
  if (token == kChannels) {
    ret = parse_channel_order(file, joint);

    if (ret)
      return ret;
  } else {
    LOG(ERROR) << "Bad structure of .bvh file. Expected " << kChannels
        << ", but found \"" << token << "\"";

    return -1;
  }

  file >> token;

  bvh_->add_joint(joint);

  //############################################################################
  // Children parsing
  //############################################################################

  while (file.good()) {
    //##########################################################################
    // Child joint parsing
    //##########################################################################
    if (token == kJoint) {
      std::shared_ptr <Joint> child;
      ret = parse_joint(file, joint, child);

      if (ret)
        return ret;

      children.push_back(child);

    //##########################################################################
    // Child joint parsing
    //##########################################################################
    } else if (token == kEnd) {
      file >> token >> token;  // Consuming "Site {"

      std::shared_ptr <Joint> tmp_joint = std::make_shared <Joint> ();

      tmp_joint->set_parent(joint);
      tmp_joint->set_name(kEndSite);
      children.push_back(tmp_joint);

      file >> token;

      //########################################################################
      // End site offset parsing
      //########################################################################
      if (token == kOffset) {
        Joint::Offset offset;

        try {
          file >> offset.x >> offset.y >> offset.z;
        } catch (const std::ios_base::failure e) {
          LOG(ERROR) << "Failure while parsing offset";
          return -1;
        }

        tmp_joint->set_offset(offset);

        LOG(DEBUG) << "Joint name : EndSite";
        LOG(TRACE) << "Offset x: " << offset.x << ", y: " << offset.y << ", z: "
            << offset.z;

        file >> token;  // Consuming "}"

      } else {
        LOG(ERROR) << "Bad structure of .bvh file. Expected " << kOffset
            << ", but found \"" << token << "\"";

        return -1;
      }

      bvh_->add_joint(tmp_joint);
    //##########################################################################
    // End joint parsing
    //##########################################################################
    } else if (token == "}") {
      joint->set_children(children);
      parsed = joint;
      return 0;
    }

    file >> token;
  }

  LOG(ERROR) << "Cannot parse joint, unexpected end of file. Last token : "
      << token;
  return -1;
}

//##############################################################################
// Motion data parse function
//##############################################################################
int Bvh_parser::parse_motion(std::ifstream& file) {

  LOG(INFO) << "Parsing motion";

  std::string token;
  file >> token;

  int frames_num;

  if (token == kFrames) {
    file >> frames_num;
    bvh_->set_num_frames(frames_num);
    LOG(DEBUG) << "Num of frames : " << frames_num;
  } else {
    LOG(ERROR) << "Bad structure of .bvh file. Expected " << kFrames
        << ", but found \"" << token << "\"";

    return -1;
  }

  file >> token;

  double frame_time;

  if (token == kFrame) {
    file >> token;  // Consuming 'Time:'
    file >> frame_time;
    bvh_->set_frame_time(frame_time);
    LOG(DEBUG) << "Frame time : " << frame_time;

    float number;
    for (int i = 0; i < frames_num; i++) {
      for (auto joint : bvh_->joints()) {
        std::vector <float> data;
        for (int j = 0; j < joint->children().size(); j++) {
          file >> number;
          data.push_back(number);
        }
        joint->add_frame_motion_data(data);
      }
    }
  } else {
    LOG(ERROR) << "Bad structure of .bvh file. Expected " << kFrame
        << ", but found \"" << token << "\"";

    return -1;
  }

  return 0;
}

//##############################################################################
// Channels order parse function
//##############################################################################
int Bvh_parser::parse_channel_order(std::ifstream& file,
    std::shared_ptr <Joint> joint) {

  LOG(TRACE) << "Parse channel order";

  int num;
  file >> num;
  LOG(TRACE) << "Number of channels : " << num;

  std::vector <Joint::Channel> channels;
  std::string token;

  for (int i = 0; i < num; i++) {
    file >> token;
    if (token == kXpos)
      channels.push_back(Joint::Channel::XPOSITION);
    else if (token == kYpos)
      channels.push_back(Joint::Channel::YPOSITION);
    else if (token == kZpos)
      channels.push_back(Joint::Channel::ZPOSITION);
    else if (token == kXrot)
      channels.push_back(Joint::Channel::XROTATION);
    else if (token == kYrot)
      channels.push_back(Joint::Channel::YROTATION);
    else if (token == kZrot)
      channels.push_back(Joint::Channel::ZROTATION);
    else {
      LOG(ERROR) << "Not valid channel!";
      return -1;
    }
  }

  joint->set_channels_order(channels);
  return 0;
}

} // namespace
