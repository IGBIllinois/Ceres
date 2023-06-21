# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindSpinnaker
----------

Find Spinnaker, which is a low-overhead, cross-platform 3D graphics
and computing API.

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` targets if Vulkan has been found:

``Vulkan::Vulkan``
  The main Vulkan library.

``Vulkan::Headers``
  .. versionadded:: 3.21

  Provides just Vulkan headers include paths, if found.  No library is
  included in this target.  This can be useful for applications that
  load Vulkan library dynamically.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables::

  Spinnaker_FOUND          - "True" if the Spinnaker SDK was found
  Spinnaker_INCLUDE_DIRS   - include directories for Spinnaker
  Spinnaker_LIBRARIES      - link against this library to use Spinnaker

The module will also define three cache variables::

  Spinnaker_INCLUDE_DIR    - the Spinnaker include directory
  Spinnaker_LIBRARY        - the path to the Spinnaker library

Hints
^^^^^

The ``SPINNAKER_SDK`` environment variable optionally specifies the
location of the Spinnaker SDK root directory for the given
architecture. 

#]=======================================================================]

## - Try to find Spinnaker library
## Once done this will define
##  SPINNAKER_FOUND - System has Spinnaker SDK installed
##  SPINNAKER_INCLUDE_DIRS - The Spinnaker include directories
#
#if (Spinnaker_INCLUDE_DIR)
#  # in cache already
#  set(Spinnaker_FOUND TRUE)
#  set(SPINNAKER_INCLUDE_DIRS "${SPINNAKER_CXX_INCLUDE_DIR}" )
#else (SPINNAKER_INCLUDE_DIR)
#
#find_path(SPINNAKER_CXX_INCLUDE_DIR 
#          NAMES asio.hpp
#          PATHS "$ENV{ASIO_HOME}/include"
#          )
#
## Used in debugging file
##message(${SPINNAKER_CXX_INCLUDE_DIR})
#
#set(SPINNAKER_INCLUDE_DIRS "${SPINNAKER_CXX_INCLUDE_DIR}" )
#
#include(FindPackageHandleStandardArgs)
## handle the QUIETLY and REQUIRED arguments and set SPINNAKER_FOUND to TRUE
## if all listed variables are TRUE
#find_package_handle_standard_args(SPINNAKER  DEFAULT_MSG
#                                  SPINNAKER_CXX_INCLUDE_DIR)
#
#mark_as_advanced(SPINNAKER_CXX_INCLUDE_DIR)
#
#endif(SPINNAKER_INCLUDE_DIR)


if(WIN32)
  find_path(Spinnaker_INCLUDE_DIR
    NAMES spinnaker/Spinnaker.h
    HINTS
      "$ENV{SPINNAKER_SDK}/Include"
    )

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_library(Spinnaker_LIBRARY
      NAMES Spinnaker_v140
      HINTS
        "$ENV{SPINNAKER_SDK}/lib64/vs2015"
        "$ENV{SPINNAKER_SDK}/bin64/vs2015"
      )
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    find_library(Spinnaker_LIBRARY
      NAMES Spinnaker_v140
      HINTS
        "$ENV{SPINNAKER_SDK}/lib/vs2015"
        "$ENV{SPINNAKER_SDK}/bin/vs2015"
      )
  endif()
else()
  find_path(Spinnaker_INCLUDE_DIR
    NAMES spinnaker/Spinnaker.h
    HINTS "$ENV{SPINNAKER_SDK}/include")
  find_library(Spinnaker_LIBRARY
    NAMES Spinnaker
    HINTS "$ENV{SPINNAKER_SDK}/lib")
endif()

set(Spinnaker_LIBRARIES ${Spinnaker_LIBRARY})
set(Spinnaker_INCLUDE_DIRS ${Spinnaker_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Spinnaker
  DEFAULT_MSG
  Spinnaker_INCLUDE_DIRS Spinnaker_LIBRARIES)

mark_as_advanced(Spinnaker_INCLUDE_DIR Spinnaker_LIBRARY)

if(Spinnaker_FOUND AND NOT TARGET Spinnaker::Spinnaker)
  add_library(Spinnaker::Spinnaker UNKNOWN IMPORTED)
  set_target_properties(Spinnaker::Spinnaker PROPERTIES
    IMPORTED_LOCATION "${Spinnaker_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${Spinnaker_INCLUDE_DIRS}")
endif()

if(Spinnaker_FOUND AND NOT TARGET Spinnaker::Headers)
  add_library(Spinnaker::Headers INTERFACE IMPORTED)
  set_target_properties(Spinnaker::Headers PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${Spinnaker_INCLUDE_DIRS}")
endif()


