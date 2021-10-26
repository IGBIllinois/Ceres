


# Add the "block data file" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/BlockDataFile)

# Add some basic utilities
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Utilities)

# Add the "experiment controller" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ExperimentControllers)

# Add the "sensors" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Sensors)

#-------------------------------------------------------------------------------
# This executable will use the Qt GUI framework.
#-------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/Resources/UI")


include("SoftwareInformation/branding.cmake")


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

qt5_add_resources(UI_RESOURCES ceres.qrc OPTIONS "--verbose")

add_executable(Ceres WIN32 MACOSX_BUNDLE)

#set_target_properties(Ceres PROPERTIES LANGUAGE C CXX)

target_compile_features(Ceres PRIVATE cxx_std_17)

target_sources(Ceres
PRIVATE
#    ${Cmake_hdr_moc}
#    ${Cmake_form_hdr}

    ${ui_files}
    ${UI_RESOURCES}
    ${platform_resource_files}

#
# General Application files
#
    ExperimentCtrlFactory.hpp
    ExperimentCtrlFactory.cpp

    SensorFactory.hpp
    SensorFactory.cpp

    ExperimentManager.hpp
    ExperimentManager.cpp
    ExperimentTreeItem.hpp
    ExperimentTreeItem.cpp
    DataModel.hpp
    DataModel.cpp
    MainWindow.hpp
    MainWindow.cpp

    main.cpp
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


#target_link_libraries(Ceres PRIVATE ${VTK_LIBRARIES})
target_link_libraries(Ceres PRIVATE ${Qt}::Widgets)
target_link_libraries(Ceres PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Ceres PRIVATE BlockDataFile)
#target_link_libraries(Ceres PRIVATE ${PCL_VISUALIZATION_LIBRARIES})
target_link_libraries(Ceres PRIVATE ExperimentControllers)
target_link_libraries(Ceres PRIVATE Sensors)
target_link_libraries(Ceres PRIVATE Utilities)

vtk_module_autoinit(TARGETS Ceres MODULES ${VTK_LIBRARIES})


#install(TARGETS asio_client RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/asio")

