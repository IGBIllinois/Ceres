
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#source_group("Resources" FILES lucid_vision_labs_communications_properties.proto)

# Generate the header/source files needed from the protobuf
#protobuf_generate_cpp(LUCID_VISION_TOF_PROTO_SRCS LUCID_VISION_TOF_PROTO_HDRS DESCRIPTORS PROTO_DESCS lucid_vision_labs_tof_properties.proto)

# Add the public header files to use the library
set(LUCID_VISION_TOF_PUBLIC_HEADERS
	LucidVisionLabsTofCameraFactory.hpp
)

# Add the public header files to use the library
set(LUCID_VISION_TOF_SOURCE_FILES

	LucidVisionLabsIDs.hpp

#
# Lucid Vision Labs packet utilities
#
#	axis_communications_packet_utils.hpp
#	axis_communications_packet_utils.cpp
	
# Lucid Vision Labs Property Page Code
#	AxisPropertiesNetDecoder.hpp
#	AxisPropertiesNetDecoder.cpp
#	AxisPropertiesNetEncoder.hpp
#	AxisPropertiesNetEncoder.cpp
#	AxisCommunicationsPropertyPage_Remote.hpp
#	AxisCommunicationsPropertyPage_Remote.cpp
#	AxisCommunicationsPropertyPage.hpp
#	AxisCommunicationsPropertyPage.cpp
	
	
# Lucid Vision Labs Controller Code

#	AxisControllerNetDecoder.hpp
#	AxisControllerNetDecoder.cpp
#	AxisControllerNetEncoder.hpp
#	AxisControllerNetEncoder.cpp
#   AxisCommunicationsController.hpp
#   AxisCommunicationsController.cpp

#
# Lucid Vision Labs Video Cameras
#
#	AxisCommunicationsView_F44.hpp
#	AxisCommunicationsView_F44.cpp
	LucidVisionLabsTofModel_Helios.hpp
	LucidVisionLabsTofModel_Helios.cpp
#	AxisCommunicationsView.hpp
#	AxisCommunicationsView.cpp
#	AxisCommunicationsStatusView.hpp
#	AxisCommunicationsStatusView.cpp
	LucidVisionLabsTofModel.hpp
	LucidVisionLabsTofModel.cpp
#	AxisCamera.hpp
#	AxisCamera.cpp
#	AxisCommunicationsUtils.hpp
#	AxisCommunicationsUtils.cpp
	LucidVisionLabsTofCameraFactory.cpp
)


#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)


# The static library (.lib/.a) for communicating with Lucid Vision Labs Time-of-Flight Cameras
add_library(lucid_vision_labs_tof_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(lucid_vision_labs_tof_static PROPERTIES OUTPUT_NAME "libLucidVisionLabsTof")
else ()
	set_target_properties(lucid_vision_labs_tof_static PROPERTIES OUTPUT_NAME "LucidVisionLabsTof")
endif (MSVC)

set_target_properties(lucid_vision_labs_tof_static PROPERTIES LANGUAGE CXX)

target_compile_features(lucid_vision_labs_tof_static PRIVATE cxx_std_20)

target_sources(lucid_vision_labs_tof_static
PUBLIC
    ${LUCID_VISION_TOF_PUBLIC_HEADERS}
 
PRIVATE
    ${LUCID_VISION_TOF_SOURCE_FILES}

    ${LUCID_VISION_TOF_PROTO_HDRS}
	${LUCID_VISION_TOF_PROTO_SRCS}
#	lucid_vision_labs_tof_properties.proto
)

target_include_directories(lucid_vision_labs_tof_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(lucid_vision_labs_tof_static PRIVATE ${RGB_INCLUDE_DIR})
target_include_directories(lucid_vision_labs_tof_static PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(lucid_vision_labs_tof_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

#target_link_libraries(lucid_vision_labs_tof_static PRIVATE cbdf::rgb)

target_link_libraries(lucid_vision_labs_tof_static PUBLIC ${Qt}::Widgets)
target_link_libraries(lucid_vision_labs_tof_static PUBLIC ${Qt}::Network)
target_link_libraries(lucid_vision_labs_tof_static PUBLIC ${Qt}::Multimedia)
target_link_libraries(lucid_vision_labs_tof_static PRIVATE ExperimentStateMachine)
target_link_libraries(lucid_vision_labs_tof_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(lucid_vision_labs_tof_static PRIVATE lucid_vision_labs_connect::LucidVisionLabsConnect_static)
target_link_libraries(lucid_vision_labs_tof_static PRIVATE ${Protobuf_LIBRARIES})

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET lucid_vision_labs_tof_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(lucid_vision_labs_tof_static PROPERTIES FOLDER "Sensors/TOF_Camera/LucidVisionLabs")
