<a id="top"></a>

# BUILDING THIRD-PARTY LIBRARIES

This file gives the basic instructions on building the third-party libraries use in this project.  All of the libraries use the cmake meta-build system.

Here is a suggested procedure to clone, build and install the third party libraries.  I do suggest building the libraries yourself because not all package managers include everything needed to work with CMake.

1. Open terminal window
2. Create a directory named github in your home directory if it does not already exist.

> mkdir github

3. Change into the github directory

> cd github

4. Clone the library

> git clone <library url>

5. Change into the newly created library directory

> cd <library name>

6. Create the build directory

> mkdir build

7. Invoke CMake to generate the build script. See [using cmake](cmake.md#top).  If you are using Windows, see note below.

> cmake -G <build system want to use> ..

8. Invoke your build system.

> make

9. Install the library.  For linux/macos, use:

> sudo make install

## Generating a Local Build

By default, the build scripts cmake generates will try to install the third party libraries into system folders.  You can instruct CMake to install to a different location by using the CMAKE_INSTALL_PREFIX variable when generating the build scripts.  On some systems, I use the `ctsndp/third_party` directory as the local install directory for the dependencies.  In that case, step seven becomes:

> cmake -G <build system want to use> -D CMAKE_INSTALL_PREFIX=<path to where you cloned the ctsndp project>/third_party ..

***Windows***

A special note to Windows users.  The default location to install the libraries is C:\Program Files\${PROJECT_NAME}.  Unfortunately, this directory is protected and Windows does not have a good sudo system in place.  I recommend creating the directory C:\Libraries and installing the third party libraries in that location.  You should add the following option when invoking cmake for the first time:
`-DCMAKE_INSTALL_PREFIX="C:\Libraries"`.

> mkdir build
> cd build
> cmake -D CMAKE_INSTALL_PREFIX="C:\Libraries" ..

Content:

* [Catch2](#catch2)
* [Clara](#clara)
* [JSON](#JSON)
* [JSON Schema Validator](#JSON Schema Validator)

## Catch2

Clone: https://github.com/catchorg/Catch2.git

Catch2 is a testing library.  However, you can use cmake to build the test suite and to do the install.  See the [README.md](https://github.com/catchorg/Catch2/blob/master/README.md) for details.  

## Clara

Clone: https://github.com/catchorg/Clara.git

Clara is a header only library so there is no real build steps.  However, you can use cmake to build the test suite.  See the [README.md](https://github.com/catchorg/Clara/blob/master/README.md) for details.  

***Linux and MacOS***
Copy the file in the single_include directory to /usr/local/include

***Windows***
Copy the file in the single_include directory to C:\Libraries\include

## JSON

Clone: https://github.com/nlohmann/json.git

The JSON library has a very simple build structure that is fully supported in the cmake build script.  See the [README.md](https://github.com/nlohmann/json/blob/master/README.md) for details.

## JSON Schema Validator

Clone: https://github.com/pboettch/json-schema-validator.git

The JSON library has a very simple build structure that is fully supported in the cmake build script.  See the [README.md](https://github.com/pboettch/json-schema-validator/blob/master/README.md) for details.  Please install the JSON library before installing this library.

***Windows***

Due to Windows not having a standard include path, you will have to tell cmake where the JSON library is located.  I am assuming you are using "C:\Libraries" to store the libraries.

> cmake -DCMAKE_INSTALL_PREFIX="C:\Libraries" -DNLOHMANN_JSON_DIR="C:\Libraries\include" ..

