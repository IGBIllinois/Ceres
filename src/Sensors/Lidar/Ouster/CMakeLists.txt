
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Resources" FILES ouster_properties.proto)

# Generate the header/source files needed from the protobuf
protobuf_generate_cpp(OUSTER_PROTO_SRCS OUSTER_PROTO_HDRS DESCRIPTORS PROTO_DESCS ouster_properties.proto)


# Add the public header files to use the library
set(OUSTER_PUBLIC_HEADERS
    OusterFactory.hpp
)

# Add the source files used in the library
set(OUSTER_SOURCE_FILES

    OusterIDs.hpp

#
# OUSTER packet utilities
#
	ouster_packet_utils.hpp
	ouster_packet_utils.cpp

# Ouster Property Page Code
	OusterPropertiesNetDecoder.hpp
	OusterPropertiesNetDecoder.cpp
	OusterPropertiesNetEncoder.hpp
	OusterPropertiesNetEncoder.cpp
	OusterPropertyPage_Remote.hpp
	OusterPropertyPage_Remote.cpp
    OusterPropertyPage_Local.hpp
    OusterPropertyPage_Local.cpp
	OusterPropertyPage.hpp
	OusterPropertyPage.cpp

    OusterExperimentStates.hpp
    OusterExperimentStates.cpp

# Ouster Controller Code

	OusterControllerNetDecoder.hpp
	OusterControllerNetDecoder.cpp
	OusterControllerNetEncoder.hpp
	OusterControllerNetEncoder.cpp
    OusterController.hpp
    OusterController.cpp

# Network stream from sensor
    OusterCmdStream.hpp
    OusterCmdStream.cpp
    OusterImuStream.hpp
    OusterImuStream.cpp
    OusterDataStream.hpp
    OusterDataStream.cpp
    OusterAsyncCommands.hpp
    OusterAsyncCommands.cpp
    OusterModel_net.hpp
    OusterModel_net.cpp

#
# Base OUSTER LiDAR files
#
    OusterView.hpp
    OusterView.cpp
    OusterStatusView.hpp
    OusterStatusView.cpp
    OusterModel.hpp
    OusterModel.cpp
    OusterFactory.cpp

)

#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)

# The static library (.lib/.a) for incorporating OUSTER LiDAR data in Ceres
add_library(ouster_sensor_static STATIC)


if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(ouster_sensor_static PROPERTIES OUTPUT_NAME "libOusterSensor")
else ()
	set_target_properties(ouster_sensor_static PROPERTIES OUTPUT_NAME "OusterSensor")
endif (MSVC)

set_target_properties(ouster_sensor_static PROPERTIES LANGUAGE CXX)

target_compile_features(ouster_sensor_static PRIVATE cxx_std_20)

target_sources(ouster_sensor_static

PUBLIC
    ${OUSTER_PUBLIC_HEADERS}
 
PRIVATE
    ${OUSTER_SOURCE_FILES}

    ${OUSTER_PROTO_HDRS}
	${OUSTER_PROTO_SRCS}
	ouster_properties.proto
)

target_include_directories(ouster_sensor_static PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(ouster_sensor_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(ouster_sensor_static PUBLIC ${Qt}::Widgets)
target_link_libraries(ouster_sensor_static PUBLIC ${Qt}::Network)
target_link_libraries(ouster_sensor_static PUBLIC ${Qt}::OpenGL)
target_link_libraries(ouster_sensor_static PUBLIC ${Qt}::OpenGLWidgets)
target_link_libraries(ouster_sensor_static PRIVATE ExperimentStateMachine)
target_link_libraries(ouster_sensor_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(ouster_sensor_static PRIVATE ouster_connect::ouster_connect_static)
target_link_libraries(ouster_sensor_static PRIVATE ouster_connect::OusterVisualization)
target_link_libraries(ouster_sensor_static PRIVATE ${Protobuf_LIBRARIES})


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET ouster_sensor_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(ouster_sensor_static PROPERTIES FOLDER "Sensors/Lidar/Ouster")
