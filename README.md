# Welcome to **Bvh Parser**, your new C++ library for .bvh file!
[![Build Status](https://travis-ci.org/BartekkPL/bvh-parser.svg?branch=master)](https://travis-ci.org/BartekkPL/bvh-parser)
=======================

  - [Features](#features)
  - [Platforms](#platforms)
  - [Usage](#usage)
    - [Build](#build)
  - [Projects using this library](#projexts-using-this-bibrary)
  - [License](#license)

## Features ##

  * Logs from working provided by [**easyloging++**](https://github.com/muflihun/easyloggingpp)
  * Clear and useful structure for bvh data
  * Tested via [**Google Test**](https://github.com/google/googletest) framework
  * Position calculation perform with [**GLM - OpenGL Mathematics**](https://github.com/g-truc/glm) library

## Platforms ##

This parser has been used only on Linux, but
it probably can works on any other platform as well.

## Usage ##

The only thing what you have to do is build and link
this library!
If you wanna know something more please look at tests or simply write mail :)

### Build ###
```
git clone https://github.com/BartekkPL/bvh-parser.git
cd bvh-parser/
mkdir build
cd build/
cmake ..
make
```

And after these steps you have builded library ready to use it in new projects!

You found it at **bvh-parser/build/lib/libbvhParser.so**.

## Projects using this library ##

I use it in my engineering thesis.

## License ##

BVH-PARSER is released under [Apache License](https://opensource.org/licenses/Apache-2.0).
