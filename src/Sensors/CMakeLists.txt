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
# General Encoder View-Model-Controller files
#
    PvtModel.hpp
    PvtModel.cpp

#
# Abstract Base Class for all Sensor Models
#
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

target_link_libraries(Sensors PRIVATE ${Qt}::Core)
target_link_libraries(Sensors PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Sensors PRIVATE gps_static)
target_link_libraries(Sensors PRIVATE lidar_static)
target_link_libraries(Sensors PRIVATE weather_static)
target_link_libraries(Sensors PRIVATE hyperspectral_static)


set_target_properties(Sensors PROPERTIES FOLDER "Sensors")


# Add the GPS library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/GPS)

# Add the Hyperspectral library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Hyperspectral)

# Add the LiDAR library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Lidar)

# Add the Weather library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Weather)


