# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindArenaSDK
----------

Find Arena SDK, which is the offical SDK from Lucid Vision Labs for their rgb/tof cameras.

IMPORTED Targets
^^^^^^^^^^^^^^^^

None

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables::

  ArenaSDK_FOUND          - "True" if the Arena SDK was found
  ArenaSDK_INCLUDE_DIRS   - include directories for Arena SDK
  ArenaSDK_LIBRARY_DIR    - the path to the Arena library
  ArenaSDK_LIBRARIES      - link against this library to use the Arena SDK

The module will also define these cache variables::

  ArenaSDK_INCLUDE_DIR    - the Arena include directory
  GenICam_INCLUDE_DIR     - include directories for GenICam library

Hints
^^^^^

The ``ARENA_SDK`` environment variable optionally specifies the
location of the ARENA SDK root directory for the given architecture. 

#]=======================================================================]


if(WIN32)
  find_path(GenICam_INCLUDE_DIR
    NAMES GenICam.h
    HINTS
      "$ENV{GENICAM}/include/"
      "$ENV{LUCID_GENICAM_PATH}/library/CPP/include/"
      "${CMAKE_INSTALL_PREFIX}/include/GenICam"
    )

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	  find_path(GenICam_LIBRARY_DIR
		NAMES GCBase_MD_VC140_v3_3_LUCID.lib
		HINTS
		  "$ENV{GENICAM}/lib64"
		  "$ENV{LUCID_GENICAM_PATH}/library/CPP/lib/Win64_x64"
		  "${CMAKE_INSTALL_PREFIX}/lib64"
		)
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	  find_path(GenICam_LIBRARY_DIR
		NAMES GCBase_MD_VC140_v3_3_LUCID.lib
		HINTS
		  "$ENV{GENICAM}/lib"
		  "$ENV{LUCID_GENICAM_PATH}/library/CPP/lib/Win32_i86"
		  "${CMAKE_INSTALL_PREFIX}/lib"
		)
  endif()

  find_path(ArenaSDK_INCLUDE_DIR
    NAMES Arena/ArenaApi.h ArenaApi.h
    HINTS
      "$ENV{ARENA_SDK}/include/"
      "$ENV{LUCID_DEV_ROOT}/include/"
      "${CMAKE_INSTALL_PREFIX}/include/ArenaSDK"
    )

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_path(ArenaSDK_LIBRARY_DIR
      NAMES Arena_v140.lib
      HINTS
        "$ENV{ARENA_SDK}/lib64/Arena"
	    "$ENV{LUCID_DEV_ROOT}/lib64/Arena"
        "${CMAKE_INSTALL_PREFIX}/lib64"
      )

  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    find_path(ArenaSDK_LIBRARY_DIR
      NAMES Arena_v140.lib
      HINTS
        "$ENV{ARENA_SDK}/lib/Arena"
	    "$ENV{LUCID_DEV_ROOT}/lib/Arena"
        "${CMAKE_INSTALL_PREFIX}/lib"
      )
  endif()

  find_library(GenICam_LIBRARY
    NAMES GCBase_MD_VC140_v3_3_LUCID.lib GenApi_MD_VC140_v3_3_LUCID.lib
    PATHS
      ${GenICam_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(Arena_LIBRARY
    NAMES Arena_v140.lib
    PATHS
      ${ArenaSDK_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(ArenaUI_LIBRARY
    NAMES ArenaUI_v140.lib
    PATHS
      ${ArenaSDK_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(GenTL_LIBRARY
    NAMES GenTL_LUCID_v140.lib
    PATHS
      ${ArenaSDK_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(Save_LIBRARY
    NAMES Save_v140.lib
    PATHS
      ${ArenaSDK_LIBRARY_DIR}
      NO_DEFAULT_PATH
    )

  find_library(GenICam_DEBUG_LIBRARY
    NAMES GCBase_MDd_VC140_v3_3_LUCID.lib  GenApi_MDd_VC140_v3_3_LUCID.lib
    PATHS
      ${GenICam_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(Arena_DEBUG_LIBRARY
    NAMES Arenad_v140.lib
    PATHS
      ${ArenaSDK_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(ArenaUI_DEBUG_LIBRARY
    NAMES ArenaUId_v140.lib
    PATHS
      ${ArenaSDK_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(GenTL_DEBUG_LIBRARY
    NAMES GenTL_LUCIDd_v140.lib
    PATHS
      ${ArenaSDK_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(Save_DEBUG_LIBRARY
    NAMES Saved_v140.lib
    PATHS
      ${ArenaSDK_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

else()
  find_path(GenICam_INCLUDE_DIR
    NAMES GenICam.h
    HINTS
    	"$ENV{GENICAM}/include"
    	"$ENV{LUCID_GENICAM_PATH}/library/CPP/include/"
    )

endif()

link_directories(${ArenaSDK_LIBRARY_DIR} ${GenICam_LIBRARY_DIR})

set(ArenaSDK_INCLUDE_DIRS ${ArenaSDK_INCLUDE_DIR} ${GenICam_INCLUDE_DIR})
set(ArenaSDK_LIBRARIES ${Arena_LIBRARY} ${ArenaUI_LIBRARY} ${GenTL_LIBRARY} ${Save_LIBRARY} ${GenICam_LIBRARY})
set(ArenaSDK_DEBUG_LIBRARIES ${Arena_DEBUG_LIBRARY} ${ArenaUI_DEBUG_LIBRARY} ${GenTL_DEBUG_LIBRARY} ${Save_DEBUG_LIBRARY} ${GenICam_DEBUG_LIBRARY})

message(STATUS "ArenaSDK_INCLUDE_DIRS=${ArenaSDK_INCLUDE_DIRS}")
message(STATUS "ArenaSDK_LIBRARY_DIR=${ArenaSDK_LIBRARY_DIR}")
message(STATUS "ArenaSDK_LIBRARIES=${ArenaSDK_LIBRARIES}")
message(STATUS "ArenaSDK_DEBUG_LIBRARIES=${ArenaSDK_DEBUG_LIBRARIES}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ArenaSDK
  DEFAULT_MSG
  ArenaSDK_INCLUDE_DIRS ArenaSDK_LIBRARIES)

mark_as_advanced(ArenaSDK_INCLUDE_DIRS ArenaSDK_LIBRARY_DIR ArenaSDK_LIBRARIES)

if(ArenaSDK_FOUND AND NOT TARGET ArenaSDK::ArenaSDK)
  add_library(ArenaSDK::ArenaSDK UNKNOWN IMPORTED)
  set_target_properties(ArenaSDK::ArenaSDK PROPERTIES
    IMPORTED_LOCATION "${ArenaSDK_LIBRARIES}"
    IMPORTED_LOCATION_DEBUG "${ArenaSDK_DEBUG_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${ArenaSDK_INCLUDE_DIRS}")
endif()

if(ArenaSDK_FOUND AND NOT TARGET ArenaSDK::Arena)
  add_library(ArenaSDK::Arena UNKNOWN IMPORTED)
  set_target_properties(ArenaSDK::Arena PROPERTIES
    IMPORTED_LOCATION "${Arena_LIBRARY}"
    IMPORTED_LOCATION_DEBUG "${Arena_DEBUG_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${ArenaSDK_INCLUDE_DIRS}")
endif()

if(ArenaSDK_FOUND AND NOT TARGET ArenaSDK::ArenaUI)
  add_library(ArenaSDK::ArenaUI UNKNOWN IMPORTED)
  set_target_properties(ArenaSDK::ArenaUI PROPERTIES
    IMPORTED_LOCATION "${ArenaUI_LIBRARY}"
    IMPORTED_LOCATION_DEBUG "${ArenaUI_DEBUG_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${ArenaSDK_INCLUDE_DIRS}")
endif()

if(ArenaSDK_FOUND AND NOT TARGET ArenaSDK::GenTL)
  add_library(ArenaSDK::GenTL UNKNOWN IMPORTED)
  set_target_properties(ArenaSDK::GenTL PROPERTIES
    IMPORTED_LOCATION "${GenTL_LIBRARY}"
    IMPORTED_LOCATION_DEBUG "${GenTL_DEBUG_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${ArenaSDK_INCLUDE_DIRS}")
endif()

if(ArenaSDK_FOUND AND NOT TARGET ArenaSDK::Save)
  add_library(ArenaSDK::Save UNKNOWN IMPORTED)
  set_target_properties(ArenaSDK::Save PROPERTIES
    IMPORTED_LOCATION "${Save_LIBRARY}"
    IMPORTED_LOCATION_DEBUG "${Save_DEBUG_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${ArenaSDK_INCLUDE_DIRS}")
endif()

if(ArenaSDK_FOUND AND NOT TARGET ArenaSDK::Headers)
  add_library(ArenaSDK::Headers INTERFACE IMPORTED)
  set_target_properties(ArenaSDK::Headers PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ArenaSDK_INCLUDE_DIRS}")
endif()


