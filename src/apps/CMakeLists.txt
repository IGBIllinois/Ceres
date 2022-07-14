
set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)

# Add the "common" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/common)

#******************************************************************************
# Installer
#******************************************************************************

# Define generic setup for all generator types...
set(CPACK_PACKAGE_NAME Ceres)
set(CPACK_PACKAGE_VENDOR "University of Illinois")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Install Ceres")
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
set(CPACK_NSIS_DISPLAY_NAME "Ceres")
set(CPACK_NSIS_PACKAGE_NAME "Ceres")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL YES)

include(CPack)

# Define components and component groups...



#******************************************************************************
# Ceres - The main data acquisition program
#******************************************************************************

message("Creating the main data acquisition application...")
add_subdirectory(Ceres)


#******************************************************************************
# Remote Client - The remote client that moves data acquisition to the dolly
#******************************************************************************

message("Creating the remote client application...")
add_subdirectory(RemoteClient)


#******************************************************************************
# Data Viewer
#******************************************************************************

message("Creating data viewer application...")
add_subdirectory(DataViewer)

