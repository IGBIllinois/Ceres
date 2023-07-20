# The QCustomPlot project will build a C++ library to show 2D plots based
# on the Qt system

# Usage:
# cmake -G <generator> -D CMAKE_INSTALL_PREFIX=<path to support libraries>

CMAKE_MINIMUM_REQUIRED(VERSION 3.18)
MESSAGE(STATUS "Found CMake ${CMAKE_VERSION}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#
# print_all_variables is a debug macro to list all of the CMAKE variables
#
macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

# Fix behavior of CMAKE_CXX_STANDARD and CMAKE_C_STANDARD when targeting macOS.
IF(POLICY CMP0025)
     CMAKE_POLICY(SET CMP0025 NEW)
ENDIF()

# Potential dangerous comparison of variables. Details: https://cmake.org/cmake/help/v3.1/policy/CMP0054.html
IF(POLICY CMP0054)
     CMAKE_POLICY(SET CMP0054 NEW)
ENDIF()

IF(POLICY CMP0071)
     CMAKE_POLICY(SET CMP0071 NEW)
ENDIF()

#
# Setting this policy to NEW allows us to use the MSVC_RUNTIME_LIBRARY 
# target property.  The generator expression would look like:
#
# To use MultiThreaded (-MT) and MultiThreadedDebug (-MTd)
# set_property(TARGET target PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#
# To use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
# set_property(TARGET target PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
#

cmake_policy(SET CMP0091 NEW)

# warn about deprecated stuff so that we may try fixing it
SET(CMAKE_WARN_DEPRECATED 1)


#******************************************************************************
# PROJECT OPTIONS
#******************************************************************************

option(QCP_BUILD_EXAMPLES "Build the QCustomPlot examples" ON)

if (MSVC)
    option(MSVC_STATIC_RUNTIME "Link static runtime libraries" OFF)
endif()


#******************************************************************************
# PROJECT LANGUAGE SUPPORT
#******************************************************************************

# We need a C and C++ compiler, so make sure project() test for it.
project(QCustomPlot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn off any compiler extensions
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

#******************************************************************************
# PROJECT DIRECTORY SUPPORT
#******************************************************************************

# Automatically include our own cmake directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# This tells cmake to search lib64, lib, ... directories in linux systems
set(CMAKE_LIBRARY_ARCHITECTURE TRUE)

# Use GNUInstallDirst to get canonical paths
include(GNUInstallDirs)

# Control where libraries and executables are placed during the build.
# With the following settings executables are placed in <the top level of the
# build tree>/bin and libraries/archives in <top level of the build tree>/lib.
# This is particularly useful to run ctests on libraries built on Windows
# machines: tests, which are executables, are placed in the same folders of
# dlls, which are treated as executables as well, so that they can properly
# find the libraries to run. This is a because of missing RPATH on Windows.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

# The clang compiler does not automatically search the /usr/local directories
if (APPLE)
   include_directories(SYSTEM "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
   link_directories("${CMAKE_INSTALL_FULL_LIBDIR}")
endif()

if ((LOCAL_INSTALL_PATH) AND (NOT LOCAL_INSTALL_PATH STREQUAL ""))
  list(APPEND CMAKE_PREFIX_PATH ${LOCAL_INSTALL_PATH})
  list(APPEND CMAKE_PREFIX_PATH "${LOCAL_INSTALL_PATH}/lib/cmake")
  list(APPEND CMAKE_PREFIX_PATH "${LOCAL_INSTALL_PATH}/lib64/cmake")
endif()

# Enable RPATH support for installed binaries and libraries
include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_FULL_BINDIR}"
                          LIB_DIRS "${CMAKE_INSTALL_FULL_LIBDIR}"
                          INSTALL_NAME_DIR "${CMAKE_INSTALL_FULL_LIBDIR}"
                          USE_LINK_PATH)

# Set the path to the public library headers
set(LIBPCUSTOMPLOT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#******************************************************************************
# PROJECT DEPENDENCIES
#******************************************************************************

# Find all the packages needed by this code

# Find and include Qt
set(Qt Qt5)

find_package(${Qt} COMPONENTS Widgets PrintSupport REQUIRED)
GET_TARGET_PROPERTY(QMAKE_LOCATION ${Qt}::qmake LOCATION)


#print_all_variables()

#******************************************************************************
# BUILD TYPE AND CODE
#******************************************************************************

# to distinguish between debug and release lib
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "Add postfix to target for Release build.")
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Add postfix to target for Debug build.")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "Add postfix to target for RelWithDebInfo build.")
set(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "Add postfix to target for MinSizeRel build")


# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()

message("CMake build is: ${CMAKE_BUILD_TYPE}")

if (MSVC)
  # Target Windows 10
  add_definitions(-D_WIN32_WINNT=0x0A00)
#  add_definitions(-D_WIN32_WINNT=0x0A00 -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

if (APPLE)
  set(CMAKE_INSTALL_NAME_DIR "@executable_path/../Libraries")
endif()

#******************************************************************************
# Libraries
#******************************************************************************
set(CMAKE_AUTOMOC ON)


set(QCUSTOMPLOT_PUBLIC_HEADERS
	qcustomplot.h
)


set(QCUSTOMPLOT_SOURCE_FILES
	qcustomplot.cpp
)


# The static library (.lib/.a) for using QCustomPlot
add_library(qcustomplot_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(qcustomplot_static PROPERTIES OUTPUT_NAME "libQCustomPlot")
else ()
	set_target_properties(qcustomplot_static PROPERTIES OUTPUT_NAME "QCustomPlot")
endif (MSVC)

set_target_properties(qcustomplot_static PROPERTIES LANGUAGE CXX)

target_compile_features(qcustomplot_static PRIVATE cxx_std_11)

target_include_directories(qcustomplot_static PUBLIC
	$<BUILD_INTERFACE:${LIBPCUSTOMPLOT_INCLUDE_DIR}>
	$<INSTALL_INTERFACE:include>
)

target_sources(qcustomplot_static
PRIVATE
	${QCUSTOMPLOT_PUBLIC_HEADERS}

PRIVATE
	${QCUSTOMPLOT_SOURCE_FILES}
)

target_link_libraries(qcustomplot_static PRIVATE ${Qt}::Widgets)
target_link_libraries(qcustomplot_static PRIVATE ${Qt}::PrintSupport)

#
# Setting MSVC_STATIC_RUNTIME determines which MSVC runtime to use...
if (MSVC_STATIC_RUNTIME)
	# To use MultiThreaded (-MT) and MultiThreadedDebug (-MTd)
	set_property(TARGET qcustomplot_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
	# To use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
	set_property(TARGET qcustomplot_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()



# The shared library (.dll/.so/.dylib) for using QCustomPlot
add_library(qcustomplot_shared SHARED)

set_target_properties(qcustomplot_shared PROPERTIES OUTPUT_NAME "QCustomPlot")

set_target_properties(qcustomplot_shared PROPERTIES LANGUAGE CXX)

target_compile_features(qcustomplot_shared PRIVATE cxx_std_11)

target_include_directories(qcustomplot_shared PUBLIC
	$<INSTALL_INTERFACE:include>
)

target_sources(qcustomplot_shared
PRIVATE
	${QCUSTOMPLOT_PUBLIC_HEADERS}

PRIVATE
	${QCUSTOMPLOT_SOURCE_FILES}
)

target_link_libraries(qcustomplot_shared PRIVATE ${Qt}::Widgets)
target_link_libraries(qcustomplot_shared PRIVATE ${Qt}::PrintSupport)

# To use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET qcustomplot_shared PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")


#******************************************************************************
# Examples
#******************************************************************************
#if (QCP_BUILD_EXAMPLES)
	add_subdirectory(examples)
#endif()


#
# Install the QCustomPlot libraries
#

install(TARGETS qcustomplot_static qcustomplot_shared
	EXPORT ${PROJECT_NAME}-config
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT ${PROJECT_NAME}-config
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(FILES ${OUSTER_PUBLIC_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

