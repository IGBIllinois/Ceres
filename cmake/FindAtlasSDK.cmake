# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindAtlasSDK
----------

Find Atlas SDK, which is the offical SDK from Teledyne for their handheld FLIR cameras.

IMPORTED Targets
^^^^^^^^^^^^^^^^

None

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables::

  AtlasSDK_FOUND          - "True" if the Atlas SDK was found
  AtlasSDK_INCLUDE_DIRS   - include directories for Atlas SDK
  AtlasSDK_LIBRARY_DIR    - the path to the Atlas library
  AtlasSDK_LIBRARIES      - link against this library to use the Atlas SDK

The module will also define these cache variables::

  AtlasSDK_INCLUDE_DIR    - the Atlas include directory

Hints
^^^^^

The ``ATLAS_SDK`` environment variable optionally specifies the
location of the ATLAS SDK root directory for the given architecture. 

#]=======================================================================]


if(WIN32)
  find_path(AtlasSDK_INCLUDE_DIR
    NAMES acs/acs.h acs.h
    HINTS
      "$ENV{ATLAS_SDK}/include/"
      "$ENV{ATLAS_DEV_ROOT}/include/"
      "${CMAKE_INSTALL_PREFIX}/include/"
    )

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_path(AtlasSDK_LIBRARY_DIR
      NAMES atlas_c_sdk.lib
      HINTS
        "$ENV{ATLAS_SDK}/lib64"
		"$ENV{ATLAS_DEV_ROOT}/lib64"
        "${CMAKE_INSTALL_PREFIX}/lib64"
      )

  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    find_path(AtlasSDK_LIBRARY_DIR
      NAMES atlas_c_sdk.lib
      HINTS
        "$ENV{ATLAS_SDK}/lib"
		"$ENV{ATLAS_DEV_ROOT}/lib"
        "${CMAKE_INSTALL_PREFIX}/lib"
      )
  endif()

  find_library(Atlas_LIBRARY
    NAMES atlas_c_sdk.lib
    PATHS
      ${AtlasSDK_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

else()
  find_path(AtlasSDK_INCLUDE_DIR
    NAMES acs/acs.h acs.h
    HINTS
	  "$ENV{ATLAS_SDK}/include"
      "$ENV{LUCID_GENICAM_PATH}/library/CPP/include/"
	)

endif()

link_directories(${AtlasSDK_LIBRARY_DIR})

set(AtlasSDK_INCLUDE_DIRS ${AtlasSDK_INCLUDE_DIR})
set(AtlasSDK_LIBRARIES ${Atlas_LIBRARY})

message(STATUS "AtlasSDK_INCLUDE_DIRS=${AtlasSDK_INCLUDE_DIRS}")
message(STATUS "AtlasSDK_LIBRARY_DIR=${AtlasSDK_LIBRARY_DIR}")
message(STATUS "AtlasSDK_LIBRARIES=${AtlasSDK_LIBRARIES}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AtlasSDK
  DEFAULT_MSG
  AtlasSDK_INCLUDE_DIRS AtlasSDK_LIBRARIES)

mark_as_advanced(AtlasSDK_INCLUDE_DIRS AtlasSDK_LIBRARY_DIR AtlasSDK_LIBRARIES)

if(AtlasSDK_FOUND AND NOT TARGET AtlasSDK::AtlasSDK)
  add_library(AtlasSDK::AtlasSDK UNKNOWN IMPORTED)
  set_target_properties(AtlasSDK::AtlasSDK PROPERTIES
    IMPORTED_LOCATION "${AtlasSDK_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${AtlasSDK_INCLUDE_DIRS}")
endif()

if(AtlasSDK_FOUND AND NOT TARGET AtlasSDK::Atlas)
  add_library(AtlasSDK::Atlas UNKNOWN IMPORTED)
  set_target_properties(AtlasSDK::Atlas PROPERTIES
    IMPORTED_LOCATION "${Atlas_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${AtlasSDK_INCLUDE_DIRS}")
endif()

if(AtlasSDK_FOUND AND NOT TARGET AtlasSDK::Headers)
  add_library(AtlasSDK::Headers INTERFACE IMPORTED)
  set_target_properties(AtlasSDK::Headers PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${AtlasSDK_INCLUDE_DIRS}")
endif()


