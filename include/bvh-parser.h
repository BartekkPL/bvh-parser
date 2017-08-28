#ifndef BVH_PARSER_H
#define BVH_PARSER_H

#include "bvh.h"
#include "joint.h"

#include <algorithm>
#include <boost/filesystem.hpp>
#include <functional>
#include <locale>
#include <memory>

namespace bf = boost::filesystem;

namespace bvh {

/** Bvh Parser class that is responsible for parsing .bvh file */
class Bvh_parser {
 public:
  /** Parses single bvh file and stored data into bvh structure
   *  @param  path  The path to file to be parsed
   *  @param  bvh   The pointer to bvh object where parsed data will be stored
   *  @return  0 if success, -1 otherwise
   */
  int parse(const bf::path& path, Bvh* bvh);

 private:
  /** Parses single hierarchy in bvh file
   *  @param  file  The input stream that is needed for reading file content
   *  @return  0 if success, -1 otherwise
   */
  int parse_hierarchy(std::ifstream& file);

  /** Parses joint and its children in bvh file
   *  @param  file    The input stream that is needed for reading file content
   *  @param  parent  The pointer to parent joint
   *  @param  parsed  The output parameter, here will be stored parsed joint
   *  @return  0 if success, -1 otherwise
   */
  int parse_joint(std::ifstream& file, std::shared_ptr <Joint> parent,
      std::shared_ptr <Joint>& parsed);

  /** Parses order of channel for single joint
   *  @param  file    The input stream that is needed for reading file content
   *  @param  joint   The pointer to joint that channels order will be parsed
   *  @return  0 if success, -1 otherwise
   */
  int parse_channel_order(std::ifstream& file, std::shared_ptr <Joint> joint);

  /** Parses motion part data
   *  @param  file    The input stream that is needed for reading file content
   *  @return  0 if success, -1 otherwise
   */
  int parse_motion(std::ifstream& file);

  /** Trims the string, removes leading and trailing whitespace from it
   *  @param  s   The string, which leading and trailing whitespace will be
   *              trimmed
   */
  inline void trim(std::string &s) {
    s.erase( std::remove_if( s.begin(), s.end(),
        std::bind( std::isspace<char>, std::placeholders::_1,
        std::locale::classic() ) ), s.end() );
  }

  /** Converts the vector of float to string, ex. "el1, el2, el3"
   *  @param  vector  The data that will be converted to string
   *  @return  The string that will be created from input data
   */
  std::string vtos(const std::vector <float> &vector);

  /** The path to file that was parsed previously */
  bf::path path_;

  /** The bvh object to store parsed data */
  Bvh* bvh_;
};

} // namespace
#endif  // BVH_PARSER_H
