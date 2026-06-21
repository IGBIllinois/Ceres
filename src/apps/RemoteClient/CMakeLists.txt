

project(CeresRemoteClient VERSION 2.0.1.0)

# Configure version resource file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/ceres_remote_client.rc.in
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/ceres_remote_client.rc
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
)

if (MSVC)
  set(platform_resource_files
    Resources/ceres_remote_client.rc
)
endif()

set(resource_files
  ceres_remote_client.qrc
)

source_group("Resources" FILES
  ${ui_files}
  ${resource_files}
  ${platform_resource_files}
)

if (${Qt} STREQUAL "Qt6")
	qt6_add_resources(UI_RESOURCES ceres_remote_client.qrc OPTIONS "--verbose")
else()
	qt5_add_resources(UI_RESOURCES ceres_remote_client.qrc OPTIONS "--verbose")
endif()

#
# Ceres Remote Data Acquition Client
#
add_executable(CeresRemoteClient WIN32 MACOSX_BUNDLE)

target_compile_features(CeresRemoteClient PRIVATE cxx_std_20)


target_sources(CeresRemoteClient
PUBLIC
#    ExperimentTypes.hpp

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

    RemoteDataModel.hpp
    RemoteDataModel.cpp
    RemoteDataThread.hpp
    RemoteDataThread.cpp
    LoopTimeMeter.hpp
    LoopTimeMeter.cpp

    RemoteClientWindow.hpp
    RemoteClientWindow.cpp
    RemoteClientCentralWindow.hpp
    RemoteClientCentralWindow.cpp

    CeresRemoteClient.cpp
)

get_source_file_property(defs AboutDialog.cpp COMPILE_DEFINITIONS)
    list(APPEND defs "CERES_REMOTE_CLIENT_VERSION=\"${CeresRemoteClient_VERSION}\"")
    set_source_files_properties(AboutDialog.cpp PROPERTIES
        COMPILE_DEFINITIONS "${defs}"
)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET CeresRemoteClient PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")


if(APPLE)
    set_target_properties(CeresRemoteClient PROPERTIES
        RESOURCE "./Resources/Icons/ripe.icns"
        MACOSX_BUNDLE_ICON_FILE ripe.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright University of Illinois"
        MACOSX_BUNDLE_GUI_IDENTIFIER "edu.illinois.ripe"
        )
endif()

target_include_directories(CeresRemoteClient PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(CeresRemoteClient PRIVATE ${SENSORS_INCLUDE_DIR})
#target_include_directories(CeresRemoteClient PRIVATE ${EXPERIMENT_CONTROLLERS_INCLUDE_DIR})
target_include_directories(CeresRemoteClient PRIVATE ${UTILITIES_INCLUDE_DIR})

target_link_libraries(CeresRemoteClient PRIVATE cbdf::cbdf)
target_link_libraries(CeresRemoteClient PRIVATE cbdf::info cbdf::ctrl)
target_link_libraries(CeresRemoteClient PRIVATE cbdf::gps cbdf::lidar cbdf::rgb cbdf::weather)

target_link_libraries(CeresRemoteClient PRIVATE ${Qt}::Widgets)
target_link_libraries(CeresRemoteClient PRIVATE ${Qt}::Network)
target_link_libraries(CeresRemoteClient PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(CeresRemoteClient PRIVATE Sensors)
target_link_libraries(CeresRemoteClient PRIVATE Utilities)
target_link_libraries(CeresRemoteClient PRIVATE common)
target_link_libraries(CeresRemoteClient PRIVATE ${Protobuf_LIBRARIES})
target_link_libraries(CeresRemoteClient PRIVATE utf8_range::utf8_range)

set_target_properties(CeresRemoteClient PROPERTIES FOLDER "Apps")

#install(TARGETS asio_client RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/asio")

