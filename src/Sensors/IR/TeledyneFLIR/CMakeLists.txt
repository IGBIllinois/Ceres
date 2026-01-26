
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Resources" FILES teledyne_flir_properties.proto)

# Generate the header/source files needed from the protobuf
protobuf_generate_cpp(TELEDYNE_FLIR_PROTO_SRCS TELEDYNE_FLIR_PROTO_HDRS DESCRIPTORS PROTO_DESCS teledyne_flir_properties.proto)

# Add the public header files to use the library
set(TELEDYNE_FLIR_PUBLIC_HEADERS
	TeledyneFlirFactory.hpp
)

# Add the public header files to use the library
set(TELEDYNE_FLIR_SOURCE_FILES

	TeledyneFlirIDs.hpp

#
# Teledyne FLIR packet utilities
#
	teledyne_flir_packet_utils.hpp
	teledyne_flir_packet_utils.cpp

#	
# Teledyne FLIR Property Page Code
#
	TeledyneFlirPropertiesNetDecoder.hpp
	TeledyneFlirPropertiesNetDecoder.cpp
	TeledyneFlirPropertiesNetEncoder.hpp
	TeledyneFlirPropertiesNetEncoder.cpp
	TeledyneFlirPropertyPage_Remote.hpp
	TeledyneFlirPropertyPage_Remote.cpp
	TeledyneFlirPropertyPage.hpp
	TeledyneFlirPropertyPage.cpp
	
	
#
# Teledyne FLIR Controller Code
#
	TeledyneFlirControllerNetDecoder.hpp
	TeledyneFlirControllerNetDecoder.cpp
	TeledyneFlirControllerNetEncoder.hpp
	TeledyneFlirControllerNetEncoder.cpp
    TeledyneFlirController.hpp
    TeledyneFlirController.cpp

#
# Teledyne FLIR Video Cameras
#
	TeledyneFlirCameraView.hpp
	TeledyneFlirCameraView.cpp
	TeledyneFlirStatusView.hpp
	TeledyneFlirStatusView.cpp
	TeledyneFlirCameraModel.hpp
	TeledyneFlirCameraModel.cpp
	TeledyneFlirCameraModel_T1300.hpp
	TeledyneFlirCameraModel_T1300.cpp
	TeledyneFlirUtils.hpp
	TeledyneFlirUtils.cpp
	TeledyneFlirFactory.cpp
)


#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)


# The static library (.lib/.a) for communicating with Teledyne FLIR Cameras
add_library(teledyne_flir_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(teledyne_flir_static PROPERTIES OUTPUT_NAME "libTeledyneFLIR")
else ()
	set_target_properties(teledyne_flir_static PROPERTIES OUTPUT_NAME "TeledyneFLIR")
endif (MSVC)

set_target_properties(teledyne_flir_static PROPERTIES LANGUAGE CXX)

target_compile_features(teledyne_flir_static PRIVATE cxx_std_20)

target_sources(teledyne_flir_static
PUBLIC
    ${TELEDYNE_FLIR_PUBLIC_HEADERS}
 
PRIVATE
    ${TELEDYNE_FLIR_SOURCE_FILES}

    ${TELEDYNE_FLIR_PROTO_HDRS}
	${TELEDYNE_FLIR_PROTO_SRCS}
	teledyne_flir_properties.proto
)

target_include_directories(teledyne_flir_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(teledyne_flir_static PRIVATE ${IR_INCLUDE_DIR})
target_include_directories(teledyne_flir_static PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(teledyne_flir_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(teledyne_flir_static PRIVATE cbdf::ir)

target_link_libraries(teledyne_flir_static PUBLIC ${Qt}::Widgets)
target_link_libraries(teledyne_flir_static PUBLIC ${Qt}::Network)
target_link_libraries(teledyne_flir_static PUBLIC ${Qt}::Multimedia)
target_link_libraries(teledyne_flir_static PRIVATE TeledyneAtlasConnect::TeledyneAtlasConnect_static)
target_link_libraries(teledyne_flir_static PRIVATE ExperimentStateMachine)
target_link_libraries(teledyne_flir_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(teledyne_flir_static PRIVATE ${Protobuf_LIBRARIES})

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET teledyne_flir_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(teledyne_flir_static PROPERTIES FOLDER "Sensors/IR_Camera/TeledyneFLIR")
