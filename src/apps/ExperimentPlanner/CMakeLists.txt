

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
    Resources/ceres_experiment_planner.rc
)
endif()

set(resource_files
  ceres_experiment_planner.qrc
)

source_group("Resources" FILES
  ${ui_files}
  ${resource_files}
  ${platform_resource_files}
)

qt5_add_resources(UI_RESOURCES ceres_experiment_planner.qrc OPTIONS "--verbose")

#
# Main Ceres Experiment Planner Application
#
add_executable(CeresExperimentPlanner WIN32 MACOSX_BUNDLE)

#set_target_properties(Ceres PROPERTIES LANGUAGE C CXX)

target_compile_features(CeresExperimentPlanner PRIVATE cxx_std_20)

target_sources(CeresExperimentPlanner
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
    ExperimentMetaInfoDlg.hpp
    ExperimentMetaInfoDlg.cpp
    ExperimentCtrlInfoDlg.hpp
    ExperimentCtrlInfoDlg.cpp
    ExperimentSensorInfoDlg.hpp
    ExperimentSensorInfoDlg.cpp

    DelayStepInfoDlg.hpp
    DelayStepInfoDlg.cpp

    MovementStepInfoDlg.hpp
    MovementStepInfoDlg.cpp

    ExperimentDesignMdiChild.hpp
    ExperimentDesignMdiChild.cpp

    ExperimentDesignWidget.hpp
    ExperimentDesignWidget.cpp

    HySpexCommandDlg.hpp
    HySpexCommandDlg.cpp

    ExperimentDesignItems.hpp
    ExperimentDesignItems.cpp

    ExperimentFile.hpp
    ExperimentFile.cpp
    ExperimentMetaInfo.hpp
    ExperimentMetaInfo.cpp
    ExperimentCtrlInfo.hpp
    ExperimentCtrlInfo.cpp
    ExperimentSensorInfo.hpp
    ExperimentSensorInfo.cpp
    ExperimentSteps.hpp
    ExperimentSteps.cpp
    ExperimentSteps_HySpex.hpp
    ExperimentSteps_HySpex.cpp

    GpsFileReader.hpp
    GpsFileReader.cpp
	
    CreateLidarExperimentDlg.hpp
    CreateLidarExperimentDlg.cpp
    CreateExperimentFromSpiderCamPointDlg.hpp
    CreateExperimentFromSpiderCamPointDlg.cpp
    CreateExperimentFromGpsDlg.hpp
    CreateExperimentFromGpsDlg.cpp
    CreateExperimentFromPlotInfoDlg.hpp
    CreateExperimentFromPlotInfoDlg.cpp

    CreateHyperspectralExperimentDlg.hpp
    CreateHyperspectralExperimentDlg.cpp

    ExperimentFieldLayoutDlg.hpp
    ExperimentFieldLayoutDlg.cpp

    ExperimentManager.hpp
    ExperimentManager.cpp
    ExperimentTreeItem.hpp
    ExperimentTreeItem.cpp

	FieldUtils.hpp
	FieldUtils.cpp

	GpsFileReader.hpp
	GpsFileReader.cpp

	RappTriangle.hpp
	RappTriangle.cpp

	RappGroundModel.hpp
	RappGroundModel.cpp

	QuadTree.hpp
	QuadTree.cpp
	QuadNode.hpp
	QuadNode.cpp

    FieldBoundaryDlg.hpp
    FieldBoundaryDlg.cpp

    FieldLayoutDlg.hpp
    FieldLayoutDlg.cpp

    FieldLayoutWidget.hpp
    FieldLayoutWidget.cpp

    ScanPreviewWidget.hpp
    ScanPreviewWidget.cpp

    MainWindow.hpp
    MainWindow.cpp

    CeresExperimentPlanner.cpp
)


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET CeresExperimentPlanner PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")


if(APPLE)
    set_target_properties(CeresExperimentPlanner PROPERTIES
        RESOURCE "./Resources/Icons/ripe.icns"
        MACOSX_BUNDLE_ICON_FILE ripe.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright University of Illinois"
        MACOSX_BUNDLE_GUI_IDENTIFIER "edu.illinois.ripe"
        )
endif()

target_include_directories(CeresExperimentPlanner PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(CeresExperimentPlanner PRIVATE ${EXPERIMENT_CONTROLLERS_INCLUDE_DIR})
target_include_directories(CeresExperimentPlanner PRIVATE ${SENSORS_INCLUDE_DIR})

target_link_libraries(CeresExperimentPlanner PRIVATE ${Qt}::Widgets)
target_link_libraries(CeresExperimentPlanner PRIVATE ${Qt}::Multimedia)
target_link_libraries(CeresExperimentPlanner PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(CeresExperimentPlanner PRIVATE Utilities)
target_link_libraries(CeresExperimentPlanner PRIVATE ExperimentControllers)
target_link_libraries(CeresExperimentPlanner PRIVATE ${VTK_LIBRARIES} )

vtk_module_autoinit(TARGETS CeresExperimentPlanner MODULES ${VTK_LIBRARIES})

set_target_properties(CeresExperimentPlanner PROPERTIES FOLDER "Apps")

#install(TARGETS asio_client RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/asio")

