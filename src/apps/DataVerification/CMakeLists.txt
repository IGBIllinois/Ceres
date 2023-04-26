
project(CeresDataVerification VERSION 1.0.0.0)


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

	DataVerifier.hpp
	DataVerifier.cpp
	
	CentralWidget.hpp
	CentralWidget.cpp
	MainWindow.hpp
	MainWindow.cpp
	
	CeresDataVerification.cpp
)


target_include_directories(CeresDataVerification PRIVATE ${CMAKE_INSTALL_PREFIX}/include)
target_include_directories(CeresDataVerification PRIVATE ${UTILITIES_INCLUDE_DIR})

if(USE_INTERNAL_CBDF_LIB)
	target_include_directories(CeresDataVerification PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})
	
	target_link_libraries(CeresDataVerification PRIVATE block_data_file_static)
else()
	target_link_libraries(CeresDataVerification PRIVATE cbdf::cbdf)

endif()

target_link_libraries(CeresDataVerification PRIVATE ${Qt}::Widgets)
target_link_libraries(CeresDataVerification PRIVATE ${Qt}::Core)

target_link_libraries(CeresDataVerification PRIVATE Utilities)


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET CeresDataVerification PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")


set_target_properties(CeresDataVerification PROPERTIES FOLDER "Apps")


#******************************************************************************
# Installer
#******************************************************************************


#install(TARGETS CeresDataVerification DESTINATION bin)

set(INSTALL_DIR bin/CeresDataVerification)
install(TARGETS CeresDataVerification DESTINATION ${INSTALL_DIR})

set(APPS \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/CeresDataVerification${CMAKE_EXECUTABLE_SUFFIX})

#qt_generate_deploy_app_script(
#    TARGET CeresDataVerification
#    FILENAME_VARIABLE datat_viewer_deploy_script
#    NO_UNSUPPORTED_PLATFORM_ERROR
#)

#install(SCRIPT ${datat_viewer_deploy_script})

install(CODE 
    "
        include(BundleUtilities)
        fixup_bundle(\"${APPS}\" \"\" \"\")
    "
    COMPONENT Runtime)
#FIXUP_BUNDLE(CeresDataVerification)

# Define generic setup for all generator types...
set(CPACK_PACKAGE_NAME CeresDataVerification)
set(CPACK_PACKAGE_VENDOR "University of Illinois")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Install Ceres Data Verification")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_VERBATIM_VARIABLES YES)

if(WIN32)
	set(CPACK_GENERATOR NSIS)
elseif(APPLE)
	set(CPACK_GENERATOR TGZ productbuild)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(CPACK_GENERATOR TGZ RPM)
else()
	set(CPACK_GENERATOR TGZ)
endif()

# NSIS-specific configuration
#set(CPACK_NSIS_MUI_ICON ${CMAKE_CURRENT_LIST_DIR}/InstallerIcon.ico)
#set(CPACK_NSIS_MUI_UNIICON ${CMAKE_CURRENT_LIST_DIR}/UninstallerIcon.ico)
set(CPACK_NSIS_INSTALLED_ICON_NAME bin/MainApp.exe)
set(CPACK_NSIS_DISPLAY_NAME "CeresDataVerification")
set(CPACK_NSIS_PACKAGE_NAME "CeresDataVerification")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL YES)

include(CPack)

