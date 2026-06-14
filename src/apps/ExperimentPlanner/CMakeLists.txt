
project(CeresExperimentPlanner VERSION 2.0.0.0)

# Configure version resource file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/ceres_experiment_planner.rc.in
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/ceres_experiment_planner.rc
    @ONLY
)


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


if (${Qt} STREQUAL "Qt6")
	qt6_add_resources(UI_RESOURCES ceres_experiment_planner.qrc OPTIONS "--verbose")
else()
	qt5_add_resources(UI_RESOURCES ceres_experiment_planner.qrc OPTIONS "--verbose")
endif()

#
# Main Ceres Experiment Planner Application
#
add_executable(CeresExperimentPlanner WIN32 MACOSX_BUNDLE)

set_target_properties(CeresExperimentPlanner PROPERTIES LANGUAGE CXX)

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

    NewSpidercamPositionDlg.hpp
    NewSpidercamPositionDlg.cpp
    ExperimentShiftDlg.hpp
    ExperimentShiftDlg.cpp

    DelayStepInfoDlg.hpp
    DelayStepInfoDlg.cpp

    MarkerInfoDlg.hpp
    MarkerInfoDlg.cpp

    MovementStepInfoDlg.hpp
    MovementStepInfoDlg.cpp

    ExperimentDesignMdiChild.hpp
    ExperimentDesignMdiChild.cpp

    ExperimentDesignWidget.hpp
    ExperimentDesignWidget.cpp

    HySpexCommandDlg.hpp
    HySpexCommandDlg.cpp

    ReferencePointDlg.hpp
    ReferencePointDlg.cpp

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
    MeasurementSteps.hpp
    MeasurementSteps.cpp
    MeasurementSteps_HySpex.hpp
    MeasurementSteps_HySpex.cpp
    MeasurementSteps_Ssnx.hpp
    MeasurementSteps_Ssnx.cpp
    MeasurementSteps_FLIR.hpp
    MeasurementSteps_FLIR.cpp

    CreateLidarExperimentDlg.hpp
    CreateLidarExperimentDlg.cpp
    CreateLidarExperimentFromSpiderCamPointDlg.hpp
    CreateLidarExperimentFromSpiderCamPointDlg.cpp
    CreateLidarExperimentFromGpsDlg.hpp
    CreateLidarExperimentFromGpsDlg.cpp
    CreateLidarExperimentFromPlotInfoDlg.hpp
    CreateLidarExperimentFromPlotInfoDlg.cpp

    CreateHyperspectralExperimentDlg.hpp
    CreateHyperspectralExperimentDlg.cpp
    CreateHyperspectralExperimentFromSpiderCamPointDlg.hpp
    CreateHyperspectralExperimentFromSpiderCamPointDlg.cpp
    CreateHyperspectralExperimentFromGpsDlg.hpp
    CreateHyperspectralExperimentFromGpsDlg.cpp
    CreateHyperspectralExperimentFromPlotInfoDlg.hpp
    CreateHyperspectralExperimentFromPlotInfoDlg.cpp
	
    CreateHyperspectralReferenceExperimentDlg.hpp
    CreateHyperspectralReferenceExperimentDlg.cpp
    CreateHyperspectralReferenceExperimentFromSpiderCamPointDlg.hpp
    CreateHyperspectralReferenceExperimentFromSpiderCamPointDlg.cpp
    CreateHyperspectralReferenceExperimentFromGpsDlg.hpp
    CreateHyperspectralReferenceExperimentFromGpsDlg.cpp

    CreateThermalExperimentDlg.hpp
    CreateThermalExperimentDlg.cpp
    CreateThermalExperimentFromSpiderCamPointDlg.hpp
    CreateThermalExperimentFromSpiderCamPointDlg.cpp

    ExperimentFieldLayoutDlg.hpp
    ExperimentFieldLayoutDlg.cpp

    MeasurementManager.hpp
    MeasurementManager.cpp
    MeasurementTreeItem.hpp
    MeasurementTreeItem.cpp

    ComputeSpidercamHeightDlg.hpp
    ComputeSpidercamHeightDlg.cpp

    ComputeSensorRangeDlg.hpp
    ComputeSensorRangeDlg.cpp

    ComputeReferenceHeightDlg.hpp
    ComputeReferenceHeightDlg.cpp

    ComputeGroundHeightDlg.hpp
    ComputeGroundHeightDlg.cpp

    RecomputeSpidercamHeightDlg.hpp
    RecomputeSpidercamHeightDlg.cpp

	FieldUtils.hpp
	FieldUtils.cpp

    FieldBoundaryDlg.hpp
    FieldBoundaryDlg.cpp

    FieldLayoutDlg.hpp
    FieldLayoutDlg.cpp

    FieldLayoutWidget.hpp
    FieldLayoutWidget.cpp

    ScanPreviewWidget.hpp
    ScanPreviewWidget.cpp

	PlannerDataModel.hpp
	PlannerDataModel.cpp
	PlannerDataModelLocal.hpp
	PlannerDataModelLocal.cpp
	PlannerDataThread.hpp
	PlannerDataThread.cpp
	
    AboutDialog.hpp
    AboutDialog.cpp

    MainWindow.hpp
    MainWindow.cpp

    CeresExperimentPlanner.cpp
)

get_source_file_property(defs AboutDialog.cpp COMPILE_DEFINITIONS)
    list(APPEND defs "CERES_EXPERIMENT_PLANNER_VERSION=\"${CeresExperimentPlanner_VERSION}\"")
    set_source_files_properties(AboutDialog.cpp PROPERTIES
        COMPILE_DEFINITIONS "${defs}"
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

#target_include_directories(CeresExperimentPlanner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CeresExperimentPlanner PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(CeresExperimentPlanner PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(CeresExperimentPlanner PRIVATE ${EXPERIMENT_CONTROLLERS_INCLUDE_DIR})
target_include_directories(CeresExperimentPlanner PRIVATE ${SENSORS_INCLUDE_DIR})

target_link_libraries(CeresExperimentPlanner PRIVATE ${Qt}::Widgets)
target_link_libraries(CeresExperimentPlanner PRIVATE ${Qt}::Multimedia)
target_link_libraries(CeresExperimentPlanner PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(CeresExperimentPlanner PRIVATE Utilities)
target_link_libraries(CeresExperimentPlanner PRIVATE ExperimentControllers)
target_link_libraries(CeresExperimentPlanner PRIVATE ${VTK_LIBRARIES} )
target_link_libraries(CeresExperimentPlanner PRIVATE common)
target_link_libraries(CeresExperimentPlanner PRIVATE ${Protobuf_LIBRARIES})
target_link_libraries(CeresExperimentPlanner PRIVATE utf8_range::utf8_range)

vtk_module_autoinit(TARGETS CeresExperimentPlanner MODULES ${VTK_LIBRARIES})

set_target_properties(CeresExperimentPlanner PROPERTIES FOLDER "Apps")

#install(TARGETS asio_client RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/asio")

