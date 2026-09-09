set(CMAKE_AUTOMOC ON)


add_library(Sensors STATIC)

#set_target_properties(Sensors PROPERTIES LANGUAGE C CXX)

target_compile_features(Sensors PRIVATE cxx_std_20)

target_sources(Sensors

PUBLIC
#
# Base Headers for all Sensor classes
#
    SensorModel.hpp

PRIVATE
#
# Interfaces to be used by various sensor models
#
    SensorInterfaces.hpp

#
# General Encoder View-Model-Controller files
#
    PvtModel.hpp
    PvtModel.cpp

#
# Dummy sensor Model-View files for testing
#
    DummySensorView.hpp
    DummySensorView.cpp
    DummySensorModel.hpp
    DummySensorModel.cpp
    DummySensorIDs.hpp
    DummySensorFactory.hpp
    DummySensorFactory.cpp

#
# Abstract Base Class for all Sensor Property Pages/Controllers
#
    SensorPropertyPage.hpp
    SensorPropertyPage.cpp
    SensorController.hpp
    SensorController.cpp

#
# Base definitions for all Sensor Models
#
    SensorDefs.hpp
    SensorDefs.cpp

#
# Abstract Base Class for all Sensor Models
#
    SensorModel.hpp
    SensorModel.cpp

#
# Base Class for all Sensor Status Views
# A status view is a tab in the Remote Client
#
    SensorStatusView.hpp
    SensorStatusView.cpp
	
#
# Factory for creating the different sensor models/views
#
    SensorFactory.hpp
    SensorFactory.cpp
)

#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE SensorModel.hpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET Sensors PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_include_directories(Sensors PRIVATE ${COMMON_INCLUDE_DIR})

target_link_libraries(Sensors PRIVATE ${Qt}::Core)
target_link_libraries(Sensors PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Sensors PUBLIC gps_static)
target_link_libraries(Sensors PUBLIC lidar_static)
target_link_libraries(Sensors PUBLIC weather_static)
target_link_libraries(Sensors PUBLIC hyperspectral_static)
target_link_libraries(Sensors PUBLIC rgb_camera_static)
target_link_libraries(Sensors PUBLIC ir_camera_static)
target_link_libraries(Sensors PUBLIC multispectral_static)
target_link_libraries(Sensors PUBLIC tof_camera_static)


set_target_properties(Sensors PROPERTIES FOLDER "Sensors")


# Add the GPS library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/GPS)

# Add the Hyperspectral library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Hyperspectral)

# Add the Multispectral library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Multispectral)

# Add the LiDAR library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Lidar)

# Add the Weather library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Weather)

# Add the RGB camera library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/RGB)

# Add the IR camera library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/IR)

# Add the Time-Of-Flight camera library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/TimeOfFlight)


