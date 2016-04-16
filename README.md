#OpenMA#

##Introduction##

OpenMA is an open-source and cross-platform library for movement analysis and required data processing. The goal of OpenMA is to provide tools for scientists to perform computations and analysis based on published and existing algorithms without the need to modify their workflow. For example, most of the motion capture (mocap) system provides their own biomechanical model with an associated marker set. However, it would be beneficial to be independent of a mocap system to choose a particular model (because this one is known as better than the provided one, in terms of reliability for a specific population, etc.). The project OpenMA wants to provide such possibilities.

##Documentation##

The user documentation is centralized and accessible over our website [openma.org](http://openma.org). The developer documentation is available on the Github [wiki](https://github.com/OpenMA/openma/wiki).

##Programming languages##

Most of the code is written in C++. If this language is mostly used in the industry, this is not the case in research laboratories. To tackle this problem, the C++ code was wrapped to different scientific programming languages.

 - Matlab toolbox composed of packages and classes
 - Python 2 & 3 packages composed of classes (coming soon)

The API for binding languages is as close as possible to the C++ API. But when possible it uses native types (e.g. matrices, cells, etc.).

##Technologies##

This project uses lots of features introduced in the **C++11 standard** (variadic template, smart pointer, unordered map, auto keyword, etc.). For this reason, a compiler supporting the C++11 standard must be used (e.g MSVC 2013, GCC 4.8, or clang 3.3). The application [CMake](http://www.cmake.org) used for the build system must be at least the release version 3.1.

The numerical computation is realized internally by the [Eigen](http://eigen.tuxfamily.org) library that uses **template expression**. On top of Eigen, OpenMA propose the nested namespace `math`. Several types and operations are proposed to handle data occlusion automatically. Thus, the implementation of numerical analysis is simplified (e.g. no need of ```for``` loop to process each sample, or to know if the sample is occluded or not).

The **opaque pointer** technique (also known as the pimpl idiom) is used in order to keep ABI compatibility between minor release of OpenMA. This point is important for third-party software (and plugins) which uses OpenMA as dynamic libraries.

The internal data structure relies on a **dynamic hierarchical tree structure** (i.e. a collection of nodes). This kind of structure has the advantage to be very flexible. Indeed, the addition of new kinds of data type is possible without modifying the existing internal structure.

The operations on files are realized using the **memory-mapped** mechanism to increase input/output performance. Thus, the data loading is faster for files physically present on-disk (or present on the network and temporary downloaded on your computer).

##Development testing##

In order to ensure the development and its quality over supported operating systems and processors, automated processes are executed each time modification in the code is done on the server. Hence, it strengthens the development, but also reduce undetected errors (bugs, missing documentation, etc.) when a new version is released.

###Continuous integration###

This part compiles the code on different operating systems in release mode and verify that all units tests passed. The unit tests are implemented using a modified version of [CxxTest](http://cxxtest.com) embedded into the source of OpenMA.

 - **MacOS X [![Build Status](https://travis-ci.org/OpenMA/openma.svg?branch=master)](https://travis-ci.org/OpenMA/openma)**
  - Compiler(s): Clang >= 3.1 / GCC >= 4.8
  - Processor architecture(s): x86_64
  - Libraries build mode(s): Static / Shared

 - **Linux [![Build Status](https://drone.io/github.com/OpenMA/openma/status.png)](https://drone.io/github.com/OpenMA/openma/latest)**
  - Compiler(s): GCC >= 4.8
  - Processor architecture(s): x86_64
  - Libraries build mode(s): Shared

 - **Microsoft Windows [![Build status](https://ci.appveyor.com/api/projects/status/1w7h67al1irf79lu?svg=true)](https://ci.appveyor.com/project/Alzathar/openma)**
  - Compiler(s): MSVC 2013
  - Processor architecture(s): x86 / x86_64
  - Libraries build mode(s): Shared

###Static code analysis [![Coverity Scan Build Status](https://scan.coverity.com/projects/8584/badge.svg)](https://scan.coverity.com/projects/8584)###

This part verifies the quality of the implementation (i.e. no lexical, syntactic, and semantic mistakes which could crash the software or create vulnerabilities) as well as its maintenance.

###Test coverage [![Coveralls Coverage Status](https://coveralls.io/repos/github/OpenMA/openma/badge.svg?branch=master)](https://coveralls.io/github/OpenMA/openma?branch=master)###

To be sure that unit tests executed in the *continuous integration* part covers all the classes/functions proposed in OpenMA, a coverage analysis is realized. It gives a metrics regarding the number of calls used in the unit tests for each methods/functions.

###API documentation [![Doxygen Coverage Status](http://drone.io/github.com/OpenMA/openma/files/build/doxy-coverage.svg)](http://drone.io/github.com/OpenMA/openma/files/build/doxy-coverage.resume.txt)###
The documentation of the API is built each time a commit is sent on the server. Internally, [Doxygen](http://www.doxygen.org) is used to verify and generate documentation of the C++ code (classes, methods, etc.). Then, a custom engine is used to generate the online documentation available on [openma.org](http://openma.org).

##License##

OpenMA use the generous open-source New BSD license. Yes, you can use OpenMA in commercial products. The complete text of the copyright is presented below:

```
Open Source Movement Analysis Library
Copyright (C) 2016, Moveck Solution Inc., all rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above
      copyright notice, this list of conditions and the following
      disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials
      provided with the distribution.
    * Neither the name(s) of the copyright holders nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
```
