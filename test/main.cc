#include "gtest/gtest.h"

#include "bvh-parser.h"
#include "easylogging++.h"

#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;

INITIALIZE_EASYLOGGINGPP

namespace {

bf::path kTestSamplePath = "../test/test-bvh-files/";

}

int main(int argc, char **argv) {
  // Load configuration from file
  el::Configurations conf("../easylogging++.config");
  // Reconfigure single logger
  el::Loggers::reconfigureLogger("default", conf);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  // ::testing::InitGoogleTest(&argc, argv);
  // return RUN_ALL_TESTS();
  bvh::Bvh_parser parser;
  bvh::Bvh data;
  bf::path sample_path = "bin/example.bvh";
  parser.parse(sample_path, &data);
}

// TEST(ExampleFileTest, ParseTest) {
//   bvh::Bvh_parser parser;
//   bvh::Bvh data;
//   bf::path sample_path = kTestSamplePath / "example.bvh";
//   ASSERT_EQ(0, parser.parse(sample_path, &data));
// }