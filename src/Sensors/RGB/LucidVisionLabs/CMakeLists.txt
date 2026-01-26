
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Resources" FILES lucid_vision_labs_rgb_properties.proto)

# Generate the header/source files needed from the protobuf
protobuf_generate_cpp(LUCID_VISION_RGB_PROTO_SRCS LUCID_VISION_RGB_PROTO_HDRS DESCRIPTORS PROTO_DESCS lucid_vision_labs_rgb_properties.proto)

# Add the public header files to use the library
set(LUCID_VISION_RGB_PUBLIC_HEADERS
	LucidVisionLabsRgbCameraFactory.hpp
)

# Add the public header files to use the library
set(LUCID_VISION_RGB_SOURCE_FILES

	LucidVisionLabsIDs.hpp

#
# Lucid Vision Labs packet utilities
#
	lucid_vision_labs_rgb_packet_utils.hpp
	lucid_vision_labs_rgb_packet_utils.cpp

#	
# Lucid Vision Labs Property Page Code
#
	LucidRgbPropertiesNetDecoder.hpp
	LucidRgbPropertiesNetDecoder.cpp
	LucidRgbPropertiesNetEncoder.hpp
	LucidRgbPropertiesNetEncoder.cpp
	LucidVisionLabsRgbPropertyPage_Remote.hpp
	LucidVisionLabsRgbPropertyPage_Remote.cpp
	LucidVisionLabsRgbPropertyPage.hpp
	LucidVisionLabsRgbPropertyPage.cpp
	
#	
# Lucid Vision Labs Controller Code
#
	LucidRgbControllerNetDecoder.hpp
	LucidRgbControllerNetDecoder.cpp
	LucidRgbControllerNetEncoder.hpp
	LucidRgbControllerNetEncoder.cpp
    LucidVisionLabsRgbController.hpp
    LucidVisionLabsRgbController.cpp

#
# Lucid Vision Labs RGB Cameras
#
	LucidVisionLabsRgbView_Triton.hpp
	LucidVisionLabsRgbView_Triton.cpp
	LucidVisionLabsRgbModel_Triton.hpp
	LucidVisionLabsRgbModel_Triton.cpp
	LucidVisionLabsRgbView.hpp
	LucidVisionLabsRgbView.cpp
	LucidVisionLabsRgbStatusView.hpp
	LucidVisionLabsRgbStatusView.cpp
	LucidVisionLabsRgbModel.hpp
	LucidVisionLabsRgbModel.cpp
	TritonCamera.hpp
	TritonCamera.cpp
	LucidVisionLabsRgbUtils.hpp
	LucidVisionLabsRgbUtils.cpp
	LucidVisionLabsRgbCameraFactory.cpp
)


#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)


# The static library (.lib/.a) for communicating with Lucid Vision Labs RGB Cameras
add_library(lucid_vision_labs_rgb_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(lucid_vision_labs_rgb_static PROPERTIES OUTPUT_NAME "libLucidVisionLabsRgb")
else ()
	set_target_properties(lucid_vision_labs_rgb_static PROPERTIES OUTPUT_NAME "LucidVisionLabsRgb")
endif (MSVC)

set_target_properties(lucid_vision_labs_rgb_static PROPERTIES LANGUAGE CXX)

target_compile_features(lucid_vision_labs_rgb_static PRIVATE cxx_std_20)

target_sources(lucid_vision_labs_rgb_static
PUBLIC
    ${LUCID_VISION_RGB_PUBLIC_HEADERS}
 
PRIVATE
    ${LUCID_VISION_RGB_SOURCE_FILES}

    ${LUCID_VISION_RGB_PROTO_HDRS}
	${LUCID_VISION_RGB_PROTO_SRCS}
#	lucid_vision_labs_properties.proto
)

target_include_directories(lucid_vision_labs_rgb_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(lucid_vision_labs_rgb_static PRIVATE ${RGB_INCLUDE_DIR})
target_include_directories(lucid_vision_labs_rgb_static PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(lucid_vision_labs_rgb_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

#target_link_libraries(lucid_vision_labs_rgb_static PRIVATE cbdf::rgb)

target_link_libraries(lucid_vision_labs_rgb_static PUBLIC ${Qt}::Widgets)
target_link_libraries(lucid_vision_labs_rgb_static PUBLIC ${Qt}::Network)
target_link_libraries(lucid_vision_labs_rgb_static PUBLIC ${Qt}::Multimedia)
target_link_libraries(lucid_vision_labs_rgb_static PRIVATE ExperimentStateMachine)
target_link_libraries(lucid_vision_labs_rgb_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(lucid_vision_labs_rgb_static PRIVATE LucidVisionLabsConnect::LucidVisionLabsConnect_static)
target_link_libraries(lucid_vision_labs_rgb_static PRIVATE ${Protobuf_LIBRARIES})

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET lucid_vision_labs_rgb_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(lucid_vision_labs_rgb_static PROPERTIES FOLDER "Sensors/RGB_Camera/LucidVisionLabs")
