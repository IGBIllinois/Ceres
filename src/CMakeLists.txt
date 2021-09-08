

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

qt5_add_resources(UI_RESOURCES ceres.qrc)

add_executable(Ceres WIN32 MACOSX_BUNDLE)

#set_target_properties(Ceres PROPERTIES LANGUAGE C CXX)

target_compile_features(Ceres PRIVATE cxx_std_17)

target_sources(Ceres
PRIVATE
    ${Cmake_hdr_moc}
    ${Cmake_form_hdr}

    ${ui_files}
    ${UI_RESOURCES}
    ${platform_resource_files}

#
# OUSTER LiDAR files
#
    OusterView.hpp
    OusterView.cpp
    LidarStreamOusterCmd.hpp
    LidarStreamOusterCmd.cpp
    LidarStreamOusterImu.hpp
    LidarStreamOusterImu.cpp
    LidarStreamOuster.hpp
    LidarStreamOuster.cpp
    LidarModelOuster.hpp
    LidarModelOuster.cpp

#
# General LiDAR View-Model-Controller files
#
    LidarModel.hpp
    LidarModel.cpp

#
# Septertino GPS receiver files
#
    GpsStreamSsnx.hpp
    GpsStreamSsnx.cpp
    GpsModelSsnx.hpp
    GpsModelSsnx.cpp

#
# General GPS View-Model-Controller files
#
    GpsView.hpp
    GpsView.cpp
    GpsModel.hpp
    GpsModel.cpp


#
# General Encoder View-Model-Controller files
#
    PvtModel.hpp
    PvtModel.cpp

#
# Abstract Base Class for all Sensor Models
#
    SensorModel.hpp
    SensorModel.cpp
    SensorFactory.hpp
    SensorFactory.cpp

#
# General Application files
#
    Constants.hpp
    DataFileBlockIdentifiers.hpp
    DataBuffer.hpp
    DataBuffer.cpp
    DataFile.hpp
    DataFile.cpp
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


target_link_libraries(Ceres PRIVATE ${VTK_LIBRARIES})
target_link_libraries(Ceres PRIVATE ${Qt}::Widgets)
target_link_libraries(Ceres PUBLIC ${Qt}::Network)
target_link_libraries(Ceres PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Ceres PRIVATE ouster::ouster_static)
target_link_libraries(Ceres PRIVATE ssnx2net::ssnx2net_static)

vtk_module_autoinit(TARGETS Ceres MODULES ${VTK_LIBRARIES})


#install(TARGETS asio_client RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/asio")

