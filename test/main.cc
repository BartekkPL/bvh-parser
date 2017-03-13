#include "gtest/gtest.h"

#include "bvh-parser.h"
#include "config.h"
#include "easylogging++.h"

#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  // Load configuration from file
  el::Configurations conf(EASYLOGGING_CONFIG_FILE_PATH);
  // Reconfigure single logger
  el::Loggers::reconfigureLogger("default", conf);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ExampleFileTest, ParseTest) {
  bvh::Bvh_parser parser;
  bvh::Bvh data;
  bf::path sample_path = bf::path(TEST_BVH_FILES_PATH) / "example.bvh";
  ASSERT_EQ(0, parser.parse(sample_path, &data));
  ASSERT_NE(nullptr, data.root_joint());
  ASSERT_STREQ("Hips", data.root_joint()->name().c_str());

  std::vector<std::vector<float>> expected_root_joint_data =
      {{8.03, 35.01,88.36, -3.41, 14.78, -164.35},
      {7.81, 35.10, 86.47, -3.78, 12.94, -166.97}};
  ASSERT_EQ(expected_root_joint_data, data.root_joint()->channel_data());
}