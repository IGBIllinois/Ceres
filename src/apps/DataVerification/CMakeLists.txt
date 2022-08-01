
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
    Resources/CeresDataVerification.rc
)
endif()

set(resource_files
  CeresDataVerification.qrc
)

source_group("Resources" FILES
  ${ui_files}
  ${resource_files}
  ${platform_resource_files}
)

qt5_add_resources(UI_RESOURCES CeresDataVerification.qrc OPTIONS "--verbose")


add_executable(CeresDataVerification WIN32 MACOSX_BUNDLE)

set_target_properties(CeresDataVerification PROPERTIES LANGUAGE CXX)

target_compile_features(CeresDataVerification PRIVATE cxx_std_20)

target_sources(CeresDataVerification
PRIVATE

    ${ui_files}
    ${UI_RESOURCES}
    ${platform_resource_files}

	OusterTypes.hpp
	OusterParser.hpp
	OusterParser.cpp
	AxisCommunicationsParser.hpp
	AxisCommunicationsParser.cpp
	ParserExceptions.hpp

	DataVerifier.hpp
	DataVerifier.cpp
	
	CentralWidget.hpp
	CentralWidget.cpp
	MainWindow.hpp
	MainWindow.cpp
	
	CeresDataVerification.cpp
)


target_include_directories(CeresDataVerification PRIVATE ${CMAKE_INSTALL_PREFIX}/include)
target_include_directories(CeresDataVerification PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})
target_include_directories(CeresDataVerification PRIVATE ${UTILITIES_INCLUDE_DIR})


target_link_libraries(CeresDataVerification PRIVATE ${Qt}::Widgets)
target_link_libraries(CeresDataVerification PRIVATE ${Qt}::Core)

target_link_libraries(CeresDataVerification PRIVATE Utilities)
target_link_libraries(CeresDataVerification PRIVATE BlockDataFile)


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET CeresDataVerification PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")


set_target_properties(CeresDataVerification PROPERTIES FOLDER "Apps")

install(TARGETS CeresDataVerification DESTINATION bin)

