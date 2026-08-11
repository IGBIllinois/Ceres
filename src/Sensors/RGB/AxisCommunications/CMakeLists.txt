
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Resources" FILES axis_communications_properties.proto)

# Generate the header/source files needed from the protobuf
protobuf_generate_cpp(AXIS_COMM_PROTO_SRCS AXIS_COMM_PROTO_HDRS DESCRIPTORS PROTO_DESCS axis_communications_properties.proto)

# Add the public header files to use the library
set(AXIS_COMM_PUBLIC_HEADERS
	AxisCommunicationsFactory.hpp
)

# Add the public header files to use the library
set(AXIS_COMM_SOURCE_FILES

	AxisCommunicationsIDs.hpp

#
# Axis Communications packet utilities
#
	axis_communications_packet_utils.hpp
	axis_communications_packet_utils.cpp
	
# Axis Communications Property Page Code
	AxisPropertiesNetDecoder.hpp
	AxisPropertiesNetDecoder.cpp
	AxisPropertiesNetEncoder.hpp
	AxisPropertiesNetEncoder.cpp
	AxisCommunicationsPropertyPage_Local.hpp
	AxisCommunicationsPropertyPage_Local.cpp
	AxisCommunicationsPropertyPage_Remote.hpp
	AxisCommunicationsPropertyPage_Remote.cpp
	AxisCommunicationsPropertyPage.hpp
	AxisCommunicationsPropertyPage.cpp
	
	
# Axis Communications Controller Code

	AxisControllerNetDecoder.hpp
	AxisControllerNetDecoder.cpp
	AxisControllerNetEncoder.hpp
	AxisControllerNetEncoder.cpp
    AxisCommunicationsController.hpp
    AxisCommunicationsController.cpp

#
# Axis Communications Video Cameras
#
	AxisCommunicationsView_F44.hpp
	AxisCommunicationsView_F44.cpp
	AxisCommunicationsModel_F44.hpp
	AxisCommunicationsModel_F44.cpp
	AxisCommunicationsView.hpp
	AxisCommunicationsView.cpp
	AxisCommunicationsStatusView.hpp
	AxisCommunicationsStatusView.cpp
	AxisCommunicationsModel.hpp
	AxisCommunicationsModel.cpp
	AxisCamera.hpp
	AxisCamera.cpp
	AxisCommunicationsUtils.hpp
	AxisCommunicationsUtils.cpp
	AxisCommunicationsFactory.cpp
)


#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)


# The static library (.lib/.a) for communicating with Axis Communications Cameras
add_library(axis_communications_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(axis_communications_static PROPERTIES OUTPUT_NAME "libAxisCommunications")
else ()
	set_target_properties(axis_communications_static PROPERTIES OUTPUT_NAME "AxisCommunications")
endif (MSVC)

set_target_properties(axis_communications_static PROPERTIES LANGUAGE CXX)

target_compile_features(axis_communications_static PRIVATE cxx_std_20)

target_sources(axis_communications_static
PUBLIC
    ${AXIS_COMM_PUBLIC_HEADERS}
 
PRIVATE
    ${AXIS_COMM_SOURCE_FILES}

    ${AXIS_COMM_PROTO_HDRS}
	${AXIS_COMM_PROTO_SRCS}
	axis_communications_properties.proto
)

target_include_directories(axis_communications_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(axis_communications_static PRIVATE ${RGB_INCLUDE_DIR})
target_include_directories(axis_communications_static PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(axis_communications_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

#target_link_libraries(axis_communications_static PRIVATE cbdf::rgb)

target_link_libraries(axis_communications_static PUBLIC ${Qt}::Widgets)
target_link_libraries(axis_communications_static PUBLIC ${Qt}::Network)
target_link_libraries(axis_communications_static PUBLIC ${Qt}::Multimedia)
target_link_libraries(axis_communications_static PRIVATE ExperimentStateMachine)
target_link_libraries(axis_communications_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(axis_communications_static PRIVATE ${Protobuf_LIBRARIES})

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET axis_communications_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(axis_communications_static PROPERTIES FOLDER "Sensors/RGB_Camera/AxisCommunications")
