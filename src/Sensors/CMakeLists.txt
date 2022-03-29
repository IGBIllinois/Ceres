set(CMAKE_AUTOMOC ON)


add_library(Sensors STATIC)

#set_target_properties(Sensors PROPERTIES LANGUAGE C CXX)

target_compile_features(Sensors PRIVATE cxx_std_17)

target_sources(Sensors

PUBLIC

#
# Base Headers for all Sensor classes
#
    SensorModel.hpp
 
PRIVATE
#
# Weather Station at the Energy Farm
#
    Weather/WindSpeedAndDirection.hpp
    Weather/WindSpeedAndDirection.cpp

    Weather/WeatherDataModel_Http_Wind.hpp
    Weather/WeatherDataModel_Http_Wind.cpp

#
# Weather Station
#
    Weather/WeatherBlockId.hpp
    Weather/WeatherBlockId.cpp
    Weather/WeatherDataIdentifiers.hpp
    Weather/WeatherSerializer.hpp
    Weather/WeatherSerializer.cpp

    Weather/WeatherDataModel_Http.hpp
    Weather/WeatherDataModel_Http.cpp
    Weather/WeatherDataModel.hpp
    Weather/WeatherDataModel.cpp

    Weather/WeatherDataFactory.hpp
    Weather/WeatherDataFactory.cpp

#
# OUSTER LiDAR files
#
    Lidar/OusterView.hpp
    Lidar/OusterView.cpp
    Lidar/OusterCmdStream.hpp
    Lidar/OusterCmdStream.cpp
    Lidar/OusterImuStream.hpp
    Lidar/OusterImuStream.cpp
    Lidar/OusterDataStream.hpp
    Lidar/OusterDataStream.cpp
    Lidar/OusterModel.hpp
    Lidar/OusterModel.cpp
    Lidar/OusterBlockId.hpp
    Lidar/OusterBlockId.cpp
    Lidar/OusterDataIdentifiers.hpp
    Lidar/OusterSerializer.hpp
    Lidar/OusterSerializer.cpp


    Lidar/OusterFactory.hpp
    Lidar/OusterFactory.cpp

#
# General LiDAR View-Model-Controller files
#
    Lidar/LidarModel.hpp
    Lidar/LidarModel.cpp

#
# Septertino GPS receiver files
#
    GPS/SsnxGpsStream.hpp
    GPS/SsnxGpsStream.cpp
    GPS/SsnxModel.hpp
    GPS/SsnxModel.cpp
    GPS/SsnxView.hpp
    GPS/SsnxView.cpp
    GPS/SsnxBlockId.hpp
    GPS/SsnxBlockId.cpp
    GPS/SsnxDataIdentifiers.hpp
    GPS/SsnxSerializer.hpp
    GPS/SsnxSerializer.cpp

    GPS/SsnxFactory.hpp
    GPS/SsnxFactory.cpp

#
# General GPS View-Model-Controller files
#
    GPS/GpsView.hpp
    GPS/GpsView.cpp
    GPS/GpsModel.hpp
    GPS/GpsModel.cpp


#
# General Encoder View-Model-Controller files
#
    PvtModel.hpp
    PvtModel.cpp

#
# Abstract Base Class for all Sensor Models
#
    SensorIdentifiers.hpp
    SensorModel.hpp
    SensorModel.cpp
)

#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE SensorModel.hpp PROPERTY SKIP_AUTOMOC ON)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET Sensors PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_include_directories(Sensors PRIVATE BlockDataFile)

target_link_libraries(Sensors PRIVATE ${Qt}::Widgets)
target_link_libraries(Sensors PUBLIC ${Qt}::Network)
target_link_libraries(Sensors PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Sensors PRIVATE ouster::ouster_static)
target_link_libraries(Sensors PRIVATE ouster::OusterVisualization)
target_link_libraries(Sensors PRIVATE ssnx2net::ssnx2net_static)
target_link_libraries(Sensors PRIVATE BlockDataFile)
target_link_libraries(Sensors PRIVATE Utilities)

target_link_libraries(Sensors PRIVATE ${VTK_LIBRARIES})
target_link_libraries(Sensors PRIVATE ${PCL_VISUALIZATION_LIBRARIES})

#vtk_module_autoinit(TARGETS Sensors MODULES ${VTK_LIBRARIES})
