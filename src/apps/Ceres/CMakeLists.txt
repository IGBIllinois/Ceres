

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
  Resources/UI/GpsWidget.ui
)

if (MSVC)
  set(platform_resource_files
    Resources/ceres.rc
)
endif()

set(resource_files
  ceres.qrc
)

source_group("Resources" FILES
  ${ui_files}
  ${resource_files}
  ${platform_resource_files}
)

if (${Qt} STREQUAL "Qt6")
	qt6_add_resources(UI_RESOURCES ceres.qrc OPTIONS "--verbose")
else()
	qt5_add_resources(UI_RESOURCES ceres.qrc OPTIONS "--verbose")
endif()

#
# Main Ceres Application
#
add_executable(Ceres WIN32 MACOSX_BUNDLE)

set_target_properties(Ceres PROPERTIES LANGUAGE CXX)

target_compile_features(Ceres PRIVATE cxx_std_20)

target_sources(Ceres
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
    MeasurementManager.hpp
    MeasurementManager.cpp
    MeasurementTreeItem.hpp
    MeasurementTreeItem.cpp
    MeasurementToolbar.hpp
    MeasurementToolbar.cpp
    BatchExpConfirmDlg.hpp
    BatchExpConfirmDlg.cpp
	
    CtrlDataModelRemote.hpp
    CtrlDataModelRemote.cpp
    RemoteClientView.hpp
    RemoteClientView.cpp
	
    CtrlDataModelLocal.hpp
    CtrlDataModelLocal.cpp
    CtrlDataThread.hpp
    CtrlDataThread.cpp
    CtrlDataModel.hpp
    CtrlDataModel.cpp

    Sound.hpp
    Sound.cpp

    HobbsMeter.hpp
    HobbsMeter.cpp

    CeresOptionsDlg.hpp
    CeresOptionsDlg.cpp

    ReferenceHeightDlg.hpp
    ReferenceHeightDlg.cpp

    MainWindow.hpp
    MainWindow.cpp

    Ceres.cpp
)


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET Ceres PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")


if(APPLE)
    set_target_properties(Ceres PROPERTIES
        RESOURCE "./Resources/Icons/ripe.icns"
        MACOSX_BUNDLE_ICON_FILE ripe.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright University of Illinois"
        MACOSX_BUNDLE_GUI_IDENTIFIER "edu.illinois.ripe"
        )
endif()

target_include_directories(Ceres PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(Ceres PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(Ceres PRIVATE ${EXPERIMENT_CONTROLLERS_INCLUDE_DIR})
target_include_directories(Ceres PRIVATE ${SENSORS_INCLUDE_DIR})

target_link_libraries(Ceres PRIVATE cbdf::cbdf)
target_link_libraries(Ceres PRIVATE cbdf::info)

#target_link_libraries(Ceres PRIVATE ${VTK_LIBRARIES})
target_link_libraries(Ceres PRIVATE ${Qt}::Widgets)
target_link_libraries(Ceres PRIVATE ${Qt}::Multimedia)
target_link_libraries(Ceres PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(Ceres PRIVATE ${PCL_VISUALIZATION_LIBRARIES})
target_link_libraries(Ceres PRIVATE ExperimentControllers)
target_link_libraries(Ceres PRIVATE Sensors)
target_link_libraries(Ceres PRIVATE Utilities)
target_link_libraries(Ceres PRIVATE common)
target_link_libraries(Ceres PRIVATE SDL3::SDL3-static)
target_link_libraries(Ceres PUBLIC ${Protobuf_LIBRARIES})
target_link_libraries(Ceres PUBLIC utf8_range::utf8_range)
target_link_libraries(Ceres PUBLIC
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

#vtk_module_autoinit(TARGETS Ceres MODULES ${VTK_LIBRARIES})

set_target_properties(Ceres PROPERTIES FOLDER "Apps")

#install(TARGETS asio_client RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/asio")

