
# We need a C and C++ compiler, so make sure project() test for it.
project(CeresFieldMapper VERSION 0.9.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn off any compiler extensions
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)


# Configure version resource file
#configure_file(
#    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/ceres.rc.in
#    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/ceres.rc
#    @ONLY
#)


#-------------------------------------------------------------------------------
# This executable will use the Qt GUI framework.
#-------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/Resources/UI")


include("${CMAKE_CURRENT_SOURCE_DIR}/SoftwareInformation/branding.cmake")


set(ui_files
  Resources/UI/MainWindow.ui
  Resources/UI/AboutDialog.ui
)

if (MSVC)
  set(platform_resource_files
    Resources/ceres_field_mapper.rc
)
endif()

set(resource_files
  ceres_field_mapper.qrc
)

source_group("Resources" FILES
  ${ui_files}
  ${resource_files}
  ${platform_resource_files}
)

if (${Qt} STREQUAL "Qt6")
	qt6_add_resources(UI_RESOURCES ceres_field_mapper.qrc OPTIONS "--verbose")
else()
	qt5_add_resources(UI_RESOURCES ceres_field_mapper.qrc OPTIONS "--verbose")
endif()

#
# Main Ceres Field Mapper Application
#
add_executable(CeresFieldMapper WIN32 MACOSX_BUNDLE)

set_target_properties(CeresFieldMapper PROPERTIES LANGUAGE CXX)

target_compile_features(CeresFieldMapper PRIVATE cxx_std_20)

target_sources(CeresFieldMapper
PUBLIC

PRIVATE
#    ${Cmake_hdr_moc}
#    ${Cmake_form_hdr}

    ${ui_files}
    ${UI_RESOURCES}
    ${platform_resource_files}

#
# General Application files
#
    AboutDialog.hpp
    AboutDialog.cpp

    MainWindow.hpp
    MainWindow.cpp

    CeresFieldMapper.cpp
)

get_source_file_property(defs AboutDialog.cpp COMPILE_DEFINITIONS)
    list(APPEND defs "CERES_FIELD_MAPPER_VERSION=\"${CeresFieldMapper_VERSION}\"")
    set_source_files_properties(AboutDialog.cpp PROPERTIES
        COMPILE_DEFINITIONS "${defs}"
)


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET CeresFieldMapper PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")


if(APPLE)
    set_target_properties(CeresFieldMapper PROPERTIES
        RESOURCE "./Resources/Icons/ripe.icns"
        MACOSX_BUNDLE_ICON_FILE ripe.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright University of Illinois"
        MACOSX_BUNDLE_GUI_IDENTIFIER "edu.illinois.ripe"
        )
endif()

target_include_directories(CeresFieldMapper PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(CeresFieldMapper PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(CeresFieldMapper PRIVATE ${EXPERIMENT_CONTROLLERS_INCLUDE_DIR})
target_include_directories(CeresFieldMapper PRIVATE ${SENSORS_INCLUDE_DIR})

target_link_libraries(CeresFieldMapper PRIVATE cbdf::cbdf)
target_link_libraries(CeresFieldMapper PRIVATE cbdf::info)

#target_link_libraries(CeresFieldMapper PRIVATE ${VTK_LIBRARIES})
target_link_libraries(CeresFieldMapper PRIVATE ${Qt}::Widgets)
target_link_libraries(CeresFieldMapper PRIVATE ${Qt}::Multimedia)
target_link_libraries(CeresFieldMapper PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(CeresFieldMapper PRIVATE ${PCL_VISUALIZATION_LIBRARIES})
target_link_libraries(CeresFieldMapper PRIVATE ExperimentControllers)
target_link_libraries(CeresFieldMapper PRIVATE Sensors)
target_link_libraries(CeresFieldMapper PRIVATE Utilities)
target_link_libraries(CeresFieldMapper PRIVATE common)
target_link_libraries(CeresFieldMapper PRIVATE SDL3::SDL3-static)
target_link_libraries(CeresFieldMapper PUBLIC ${Protobuf_LIBRARIES})
target_link_libraries(CeresFieldMapper PUBLIC utf8_range::utf8_range)
target_link_libraries(CeresFieldMapper PUBLIC
    absl::absl_check
    absl::absl_log
    absl::algorithm
    absl::base
    absl::bind_front
    absl::bits
    absl::btree
    absl::cleanup
    absl::cord
    absl::core_headers
    absl::debugging
    absl::die_if_null
    absl::dynamic_annotations
    absl::flags
    absl::flat_hash_map
    absl::flat_hash_set
    absl::function_ref
    absl::hash
    absl::layout
    absl::log_initialize
    absl::log_globals
    absl::log_severity
    absl::memory
    absl::node_hash_map
    absl::node_hash_set
    absl::optional
    absl::random_distributions
    absl::random_random
    absl::span
    absl::status
    absl::statusor
    absl::strings
    absl::synchronization
    absl::time
    absl::utility
)

#vtk_module_autoinit(TARGETS CeresFieldMapper MODULES ${VTK_LIBRARIES})

set_target_properties(CeresFieldMapper PROPERTIES FOLDER "Apps")

#install(TARGETS asio_client RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/asio")

