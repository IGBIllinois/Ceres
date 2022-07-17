

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

#
# Main Ceres Application
#
add_executable(Ceres WIN32 MACOSX_BUNDLE)

#set_target_properties(Ceres PROPERTIES LANGUAGE C CXX)

target_compile_features(Ceres PRIVATE cxx_std_17)

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
    ExperimentManager.hpp
    ExperimentManager.cpp
    ExperimentTreeItem.hpp
    ExperimentTreeItem.cpp
    ExperimentToolbar.hpp
    ExperimentToolbar.cpp
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
target_include_directories(Ceres PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})
target_include_directories(Ceres PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(Ceres PRIVATE ${EXPERIMENT_CONTROLLERS_INCLUDE_DIR})
target_include_directories(Ceres PRIVATE ${SENSORS_INCLUDE_DIR})


#target_link_libraries(Ceres PRIVATE ${VTK_LIBRARIES})
target_link_libraries(Ceres PRIVATE ${Qt}::Widgets)
target_link_libraries(Ceres PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Ceres PRIVATE BlockDataFile)
#target_link_libraries(Ceres PRIVATE ${PCL_VISUALIZATION_LIBRARIES})
target_link_libraries(Ceres PRIVATE ExperimentControllers)
target_link_libraries(Ceres PRIVATE Sensors)
target_link_libraries(Ceres PRIVATE Utilities)
target_link_libraries(Ceres PRIVATE common)

#vtk_module_autoinit(TARGETS Ceres MODULES ${VTK_LIBRARIES})

set_target_properties(Ceres PROPERTIES FOLDER "Apps")

#install(TARGETS asio_client RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/asio")

