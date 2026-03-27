# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindGenICam
----------

Find GenICam, which is the offical version of the Gen<I>Cam standard from Lucid Vision Labs for their rgb/tof cameras.

IMPORTED Targets
^^^^^^^^^^^^^^^^

None

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables::

  GenICam_FOUND          - "True" if the GenICam library was found
  GenICam_INCLUDE_DIRS   - include directories for GenICam library
  GenICam_LIBRARIES      - link against this library to use the GenICam

The module will also define these cache variables::

  GENICAM_INCLUDE_DIR    - the Gen<I>Cam include directory
  GENICAM_LIBRARY_DIR    - the path to the Gen<I>Cam library

Hints
^^^^^

The ``GENICAM`` environment variable optionally specifies the
location of the GenICam root directory for the given architecture. 

#]=======================================================================]


if(WIN32)
  find_path(GENICAM_INCLUDE_DIR
    NAMES GenICam.h
    HINTS
      "$ENV{GENICAM}/include/"
      "${CMAKE_INSTALL_PREFIX}/include/GenICam"
    )

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    find_path(GENICAM_LIBRARY_DIR
      NAMES GenApi_MD_VC140_v3_3_LUCID.lib
      HINTS
        "$ENV{GENICAM}/lib/Win64_x64"
        "${CMAKE_INSTALL_PREFIX}/lib64"
      )
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    find_path(GENICAM_LIBRARY_DIR
      NAMES GenApi_MD_VC140_v3_3_LUCID.lib
      HINTS
        "$ENV{GENICAM}/lib/Win32_i86"
        "${CMAKE_INSTALL_PREFIX}/lib"
      )
  endif()

  find_library(GENAPI_LIBRARY
    NAMES GenApi_MD_VC140_v3_3_LUCID GenApi_MDd_VC140_v3_3_LUCID
    PATHS
      ${GENICAM_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(GCBASE_LIBRARY
    NAMES GCBase_MD_VC140_v3_3_LUCID GCBase_MDd_VC140_v3_3_LUCID
    PATHS
      ${GENICAM_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(LOG_LIBRARY
    NAMES Log_MD_VC140_v3_3_LUCID Log_MDd_VC140_v3_3_LUCID
    PATHS
      ${GENICAM_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(MATH_PARSER_LIBRARY
    NAMES MathParser_MD_VC140_v3_3_LUCID MathParser_MDd_VC140_v3_3_LUCID
    PATHS
      ${GENICAM_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(NODE_MAP_DATA_LIBRARY
    NAMES NodeMapData_MD_VC140_v3_3_LUCID NodeMapData_MDd_VC140_v3_3_LUCID
    PATHS
      ${GENICAM_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(RES_USAGE_STAT_PARSER_LIBRARY
    NAMES ResUsageStat_MD_VC140_v3_3_LUCID ResUsageStat_MDd_VC140_v3_3_LUCID
    PATHS
      ${GENICAM_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

  find_library(XML_PARSER_LIBRARY
    NAMES XmlParser_MD_VC140_v3_3_LUCID XmlParser_MDd_VC140_v3_3_LUCID
    PATHS
      ${GENICAM_LIBRARY_DIR}
	NO_DEFAULT_PATH
    )

else()
  find_path(GENICAM_INCLUDE_DIR
    NAMES GenICam.h
    HINTS
	  "$ENV{GENICAM}/include"
	)

  find_library(GENICAM_LIBRARY
    NAMES GCBase_MD_VC140_v3_3_LUCID
    HINTS "$ENV{GENICAM}/lib")
endif()

set(GenICam_INCLUDE_DIRS ${GENICAM_INCLUDE_DIR})
set(GenICam_LIBRARIES ${GENAPI_LIBRARY} ${GCBASE_LIBRARY} ${LOG_LIBRARY} ${MATH_PARSER_LIBRARY} ${NODE_MAP_DATA_LIBRARY} ${RES_USAGE_STAT_PARSER_LIBRARY} ${XML_PARSER_LIBRARY})

message(STATUS "GenICam_INCLUDE_DIRS=${GenICam_INCLUDE_DIRS}")
message(STATUS "GENICAM_LIBRARY_DIR=${GENICAM_LIBRARY_DIR}")
message(STATUS "GenICam_LIBRARIES=${GenICam_LIBRARIES}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GenICam
  DEFAULT_MSG
  GenICam_INCLUDE_DIRS GenICam_LIBRARIES)

mark_as_advanced(GenICam_INCLUDE_DIRS GenICam_LIBRARIES)

if(GenICam_FOUND AND NOT TARGET GenICam::GenICam)
  add_library(GenICam::GenICam UNKNOWN IMPORTED)
  set_target_properties(GenICam::GenICam PROPERTIES
    IMPORTED_LOCATION "${GenICam_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${GenICam_INCLUDE_DIRS}")
endif()

if(GenICam_FOUND AND NOT TARGET GenICam::Headers)
  add_library(GenICam::Headers INTERFACE IMPORTED)
  set_target_properties(GenICam::Headers PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${GenICam_INCLUDE_DIRS}")
endif()


