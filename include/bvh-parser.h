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

class Bvh_parser {
  public:
    /** Main parse function, that parse single bvh file into bvh structure */
    int parse(const bf::path& path, Bvh* bvh);
  private:
    /** Parse single hierarchy in bvh file */
    int parse_hierarchy(std::ifstream& file);
    /** Parse joint and its children in bvh file */
    int parse_joint(std::ifstream& file, std::shared_ptr <Joint> parent,
        std::shared_ptr <Joint>& parsed);
    /** Parse order of channel for single joint */
    int parse_channel_order(std::ifstream& file, std::shared_ptr <Joint> joint);
    int count_channels(const std::shared_ptr <Joint> joint);
    /** Parse motion data for one hierarchy */
    int parse_motion(std::ifstream& file);

    /** Returns a copy of the string, with leading and trailing whitespace
     *  omitted
     */
    inline void trim(std::string &s) {
      s.erase( std::remove_if( s.begin(), s.end(),
          std::bind( std::isspace<char>, std::placeholders::_1,
          std::locale::classic() ) ), s.end() );
    }
    /** Converts the vector to string, ex. "el1, el2, el3" */
    std::string vtos(const std::vector <float> &vector);
    bf::path path_;
    Bvh* bvh_;
};

} // namespace
#endif  // BVH_PARSER_H
