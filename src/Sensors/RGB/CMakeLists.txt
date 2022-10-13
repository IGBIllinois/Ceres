
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Resources" FILES axis_communications_properties.proto)

# Generate the header/source files needed from the protobuf
protobuf_generate_cpp(AXIS_COMM_PROTO_SRCS AXIS_COMM_PROTO_HDRS DESCRIPTORS PROTO_DESCS axis_communications_properties.proto)

# Add the public header files to use the library
set(RGB_CAMERA_PUBLIC_HEADERS
	RgbCameraFactory.hpp
)

# Add the public header files to use the library
set(RGB_CAMERA_SOURCE_FILES

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
	AxisCommunicationsModel.hpp
	AxisCommunicationsModel.cpp
	AxisCamera.hpp
	AxisCamera.cpp
	AxisCommunicationsUtils.hpp
	AxisCommunicationsUtils.cpp
	AxisCommunicationsFactory.hpp
	AxisCommunicationsFactory.cpp

#
# General RGB Camera View-Model-Controller files
#
    RgbImageWidget.hpp
    RgbImageWidget.cpp
    RgbCameraView.hpp
    RgbCameraView.cpp
    RgbCameraModel.hpp
    RgbCameraModel.cpp
	RgbCameraFactory.cpp
)


# The static library (.lib/.a) for communicating with Multispectral Cameras
add_library(rgb_camera_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(rgb_camera_static PROPERTIES OUTPUT_NAME "libRgbCamera")
else ()
	set_target_properties(rgb_camera_static PROPERTIES OUTPUT_NAME "RgbCamera")
endif (MSVC)

set_target_properties(rgb_camera_static PROPERTIES LANGUAGE CXX)

target_compile_features(rgb_camera_static PRIVATE cxx_std_17)

target_sources(rgb_camera_static
PUBLIC
    ${RGB_CAMERA_PUBLIC_HEADERS}
 
PRIVATE
    ${RGB_CAMERA_SOURCE_FILES}

    ${AXIS_COMM_PROTO_HDRS}
	${AXIS_COMM_PROTO_SRCS}
	axis_communications_properties.proto
)

target_include_directories(rgb_camera_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(rgb_camera_static PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(rgb_camera_static PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})
target_include_directories(rgb_camera_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(rgb_camera_static PUBLIC ${Qt}::Widgets)
target_link_libraries(rgb_camera_static PUBLIC ${Qt}::Network)
target_link_libraries(rgb_camera_static PUBLIC ${Qt}::Multimedia)
target_link_libraries(rgb_camera_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(rgb_camera_static PUBLIC block_data_file_static)
target_link_libraries(rgb_camera_static PRIVATE ${Protobuf_LIBRARIES})

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET rgb_camera_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(rgb_camera_static PROPERTIES FOLDER "Sensors/RGB_Camera")
