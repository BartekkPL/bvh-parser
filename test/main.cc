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
}