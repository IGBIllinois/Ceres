<a id="top"></a>

# BUILDING THE SPIDERCAM LIBRARIES

These are the instructions on building the SpiderCam libraries and tests.  We use the cmake meta-build system to generate the build system for the target operating system.  CMake encourages the use of a side-by-side build structure.  In this way, the build, object, library and executable files are kept separate from the source files use to generate them.  A side-by-side build structure look like:

    .
    ├── build             # The location of the object, library and executables
    └── src               # the source code

Before we begin, make sure you have:

1. [CMake is installed on your system](cmake.md#top).
2. Built and installed all [third party libraries](build_third_party_libs.md#top).

Here is a suggested procedure to clone, build and install the SpiderCam project.

1. Open terminal window
2. Create a directory named IGB in your home directory if it does not already exist.

> mkdir IGB

3. Change into the IGB directory

> cd IGB

4. Clone the project

> git clone https://github.com/IGBIllinois/SpiderCamConnect.git

5. Change into the newly created library directory

> cd SpiderCamConnect

6. Create the build directory

> mkdir build

7. Change into the build directory

> cd build

**Windows** 

A special note to Windows users.  The default location to install the libraries is C:\Program Files\${PROJECT_NAME}.  Unfortunately, this directory is protected and Windows does not have a good sudo system in place.  If you installed the third party libraries in the directory C:\lib, you should add the following option when invoking cmake for the first time:
`-DCMAKE_PREFIX_PATH="C:\lib"`.

> cmake -G "Visual Studio 16 2019" -DCMAKE_PREFIX_PATH=C:\lib ..

You should find a spidercam.sln file in the build directory.

## Using Make

Make is the default build tool for many operating systems: Linux, MacOS, etc.  Make can also be use in Windows environments if Cygwin or MingW64/msys are install.

> cmake -G "Unix Makefiles" ..
> make

## Using Ninja

Ninja is a cross-platform build tool like make.  However, it does provide a faster build system then make does.  If you have ninja install, you can use:

> cmake -G "Ninja" ..
> ninja

If everything builds correctly, the executable will be located in the build/bin directory.

