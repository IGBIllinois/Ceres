
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Add the public header files to use the library
set(LIDAR_PUBLIC_HEADERS
    LidarFactory.hpp
)

# Add the source files used in the library
set(LIDAR_SOURCE_FILES

#
# General LiDAR View-Model-Controller files
#
    LidarModel.hpp
    LidarModel.cpp
    LidarFactory.cpp
)


# The static library (.lib/.a) for incorporating LiDAR data in Ceres
add_library(lidar_static STATIC)


if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(lidar_static PROPERTIES OUTPUT_NAME "libLiDAR")
else ()
	set_target_properties(lidar_static PROPERTIES OUTPUT_NAME "LiDAR")
endif (MSVC)

set_target_properties(lidar_static PROPERTIES LANGUAGE CXX)

target_compile_features(lidar_static PRIVATE cxx_std_17)

target_sources(lidar_static

PUBLIC
    ${LIDAR_PUBLIC_HEADERS}
 
PRIVATE
    ${LIDAR_SOURCE_FILES}
)

#target_include_directories(lidar_static PRIVATE ${UTILITIES_INCLUDE_DIR})
#target_include_directories(lidar_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(lidar_static PRIVATE cbdf::lidar)

target_link_libraries(lidar_static PRIVATE hokuyo_sensor_static)
target_link_libraries(lidar_static PRIVATE ouster_sensor_static)
target_link_libraries(lidar_static PRIVATE nlohmann_json::nlohmann_json)


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET lidar_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(lidar_static PROPERTIES FOLDER "Sensors/Lidar")


# Add the Ouster LiDAR library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Hokuyo)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Ouster)

