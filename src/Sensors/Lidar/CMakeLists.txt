
# Add the public header files to use the library
set(LIDAR_PUBLIC_HEADERS
    OusterFactory.hpp
)

# Add the source files used in the library
set(LIDAR_SOURCE_FILES
#
# OUSTER LiDAR files
#
    OusterView.hpp
    OusterView.cpp
    OusterCmdStream.hpp
    OusterCmdStream.cpp
    OusterImuStream.hpp
    OusterImuStream.cpp
    OusterDataStream.hpp
    OusterDataStream.cpp
    OusterModel.hpp
    OusterModel.cpp
    OusterFactory.cpp

#
# General LiDAR View-Model-Controller files
#
    LidarModel.hpp
    LidarModel.cpp
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

target_include_directories(lidar_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(lidar_static PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})

target_link_libraries(lidar_static PUBLIC ${Qt}::Widgets)
target_link_libraries(lidar_static PUBLIC ${Qt}::Network)
target_link_libraries(lidar_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(lidar_static PRIVATE ouster::ouster_static)
target_link_libraries(lidar_static PRIVATE ouster::OusterVisualization)
target_link_libraries(lidar_static PUBLIC BlockDataFile)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET lidar_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(lidar_static PROPERTIES FOLDER "Sensors/Lidar")
