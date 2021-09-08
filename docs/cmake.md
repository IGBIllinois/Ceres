<a id="top"></a>

# CMake

CMake is a meta-build system developed and mantained by Kitware.  CMake does not compile your code directly, but generates/manages the require build files to compile your code.  With CMake build scripts, CMakeLists.txt, it is possible to create platform independent project descriptions and let CMake create specific project or build files for other tools.  CMake supports many OSes and build systems: Unix make files, Ninja, XCode, Visual Studio and more.

See https://cmake.org/documentation/ for more details.

For the CTSNDP code you will need CMake version 3.14 or higher.  If you already have CMake installed, use the `cmake --version` command to check the version number.

## Installing CMake

### Mac OS

The easiest way to install and maintain CMake on the Mac platform is to use "Homebrew" (https://brew.sh).  Run the following command in the terminal window to install CMake on your system:

> brew install cmake

### Ubuntu

The easiest method to install and maintain CMake on Debian / Ubuntu platforms is to use APT.  Unfortunately, the official version of cmake in the apt repositories does not always match the latest released version.  You can follow the instruction given here https://apt.kitware.com so that apt-get will use the Kitware repositories.

> sudo apt-get install cmake

### Windows

Visit the https://cmake.org/download/ to download the installer (msi file) for your version of windows. 

## Building with the CMake build script

CMake encourages the idea of a side-by-side builds.  In other words, your code might be in the "src" directory, but the generated object and executable will be stored in the "build" directory.  Note: the "build" directory is already part of the .gitignore file as anything that is generated should not be in the repo.  The directory structure should look like:

> project_directory
>   |
>   +--> src
>   |
>   +--> build
>   |

Open a terminal window.  On windows, run cmd.exe to open a terminal window.  Change to your project directory.  Typically for C/C++ projects, there should be a "src" subdirectory.  Create a new subdirectory in the project directory named "build". Change to this "build" subdirectory. You should invoke cmake here. As an command line argument you can supply a "generator" so that cmake knows which build system you will use to build the project.  

> cmake -G <generator-name> ..

Note: The ".." at the end of the command line tells cmake where to find the main build script file (called CMakeLists.txt). This should point to the project's base directory, which is in our case the parent directory to the "build" directory.  You can use `cmake --help` to see a list of generators supported by your system.  Note: the help option will produce a list of generators available on your first.  The first one, marked with an asterisk, is the default if the -G option is not used.

For example, on Windows using Visual Studio 2015 to build the project, the cmake command

> cmake -G "Visual Studio 14 2015" ..

will instruct cmake to create a "solution" for Visual Studio 2015. The resulting project files will be 32 bit.

To build 64 bit solution the "Win64" generator option has to be added.

> cmake -G "Visual Studio 14 2015 Win64" ..

 linux system, the cmake command becomes:

> cmake -G "Unix Makefiles" ..

***Windows Users***

A special note to Windows users and third party libraries.  The default location to install the libraries is C:\Program Files\${PROJECT_NAME}.  Unfortunately, this directory is protected and Windows does not have a good sudo system in place.  I recommend creating the directory C:\Libraries and installing the third party libraries in that location.  You can instruct CMake to install to a different location by using the CMAKE_INSTALL_PREFIX variable.  For example:

> cmake -G "Visual Studio 14 2015" -D CMAKE_INSTALL_PREFIX="C:\Libraries" ..

For the main program, use the LOCAL_INSTALL_PATH to set the library search path to include C:\Libraries.  ***Note: You can also use the built in CMAKE_PREFIX_PATH, but I append several directories to search based on LOCAL_INSTALL_PATH.***

> cmake -G "Visual Studio 14 2015" -D LOCAL_INSTALL_PATH="C:\Libraries" ..

## Using Local Dependencies in Build

In this section, I am assuming you have built the dependencies and installed them locally by using the `CMAKE_INSTALL_PREFIX` variable.  I am assuming the `ctsndp/third_party` directory was using for the destination of the include and library files.  If not, just replace where I use `ctsndp/third_party` with the directory you use.  In this example, I have cloned the CTSNDP project in my home directory and I can use tilda, ~, to reference my home directory.  For windows you have to use `%HOMEPATH%`.  ***Note: Do not use relative paths as CMake will be changing directories as it processes the CMakeLists.txt files.***

Generate the build scripts in the normal way to see what cmake can find on its own:

> In the build directory:
> cmake -G <generator-name> -D LOCAL_INSTALL_PATH=~/ctsndp/third_party ..

If there are any missing components, please edit the CMakeLists.txt file in `ctshdp/cpp` and add these missing directories.  Find the usage of LOCAL_INSTALL_PATH in the file for examples of how to append new directories.
