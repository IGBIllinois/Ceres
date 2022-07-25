
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
)

if (MSVC)
  set(platform_resource_files
    Resources/CeresDataViewer.rc
)
endif()

set(resource_files
  CeresDataViewer.qrc
)

source_group("Resources" FILES
  ${ui_files}
  ${resource_files}
  ${platform_resource_files}
)

qt5_add_resources(UI_RESOURCES CeresDataViewer.qrc OPTIONS "--verbose")


add_executable(CeresDataViewer WIN32 MACOSX_BUNDLE)

set_target_properties(CeresDataViewer PROPERTIES LANGUAGE CXX)

target_compile_features(CeresDataViewer PRIVATE cxx_std_20)

target_sources(CeresDataViewer
PRIVATE

    ${ui_files}
    ${UI_RESOURCES}
    ${platform_resource_files}

    AxisCommunicationsParser.hpp
    AxisCommunicationsParser.cpp
    AxisCommunicationsModel_file.hpp
    AxisCommunicationsModel_file.cpp

    OusterTypes.hpp
    OusterParser.hpp
    OusterParser.cpp
	
    SsnxTypes.hpp
    SsnxParser.hpp
    SsnxParser.cpp
    SsnxModel_file.hpp
    SsnxModel_file.cpp

	WeatherParser.hpp
	WeatherParser.cpp
	SpidercamParser.hpp
	SpidercamParser.cpp
	ExperimentParser.hpp
	ExperimentParser.cpp

	CeresDataFile.hpp
	CeresDataFile.cpp
	
	CentralWidget.hpp
	CentralWidget.cpp
	MainWindow.hpp
	MainWindow.cpp
	
	CeresDataViewer.cpp
)


target_include_directories(CeresDataViewer PRIVATE ${CMAKE_INSTALL_PREFIX}/include)
target_include_directories(CeresDataViewer PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})
target_include_directories(CeresDataViewer PRIVATE ${UTILITIES_INCLUDE_DIR})
#target_include_directories(CeresDataViewer PRIVATE ${EXPERIMENT_CONTROLLERS_INCLUDE_DIR})
target_include_directories(CeresDataViewer PRIVATE ${SENSORS_INCLUDE_DIR})


target_link_libraries(CeresDataViewer PRIVATE ${Qt}::Widgets)
target_link_libraries(CeresDataViewer PRIVATE ${Qt}::Core)

target_link_libraries(CeresDataViewer PRIVATE BlockDataFile)
target_link_libraries(CeresDataViewer PRIVATE Sensors)


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET CeresDataViewer PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")


set_target_properties(CeresDataViewer PROPERTIES FOLDER "Apps")

install(TARGETS CeresDataViewer DESTINATION bin)

