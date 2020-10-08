# Wave File to Mp3 File Encoder #


## Description ## 
------
A command line program encodes Wav files to Mp3 files using Lame and POSIX threads.
<br><br>

## Dependencies ##
---
- Lame static library version 3.100  
I have built lame from source on Ubuntu 20.04 and Win10
    - Linux: **libmp3lame.a** -- shipped within project in sub-directory /dependencyLinux
    - Windows: **libmp3lame-static.lib**; **libmpghip-static.lib**; -- shipped within project in sub-directory /dependencyWin
- pthreads
    - Linux: if not already installed, using command: sudo apt-get install libpthread-stubs0-dev
    - Windows: **pthreadVC3.lib** -- shipped within project in sub-directory /dependencyWin

## Build
---

### Linux ###
CMakeLists are provided. To build in /build directory, using following commands in project root directory:
```
mkdir build && cd build
cmake .. && make
cd src
```
Excuateable file **encoder** can be found there. 
Test with gcc 9.3.0 in Ubuntu 20.04

### Windows ###
A solution file is provided for Microsoft Visual Studio 2019. Additional include path and linker path were set relatively to the project directory. It should be built with necessary dependencies without further project configuration in Visual Studio.

Test with visual studio 16 2019 on Windows 10
The C/CXX compiler identification is MSVC 19.27.29112.0

<br><br>

## Usage ##
---
1. Application is called with path to a directory storing all wave files to be encoded as an argument, such as:  
`./encoder <path_to_dir_with_wave_files> `

<br><br>

## Notes ##
---
As default all available CPU cores on the current machine during compile time will be used in a "one core one thread" manner: each CPU core run one thread and encode one wave file. No multi-threads access one wave file at the same time.



