
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Resources" FILES hyspex_swir_384_properties.proto hyspex_vnir_3000N_properties.proto)

# Generate the header/source files needed from the protobuf
protobuf_generate_cpp(HYSPEX_SWIR384_PROTO_SRCS HYSPEX_SWIR384_PROTO_HDRS DESCRIPTORS PROTO_DESCS hyspex_swir_384_properties.proto)
protobuf_generate_cpp(HYSPEX_VNIR3000N_PROTO_SRCS HYSPEX_VNIR3000N_PROTO_HDRS DESCRIPTORS PROTO_DESCS hyspex_vnir_3000N_properties.proto)

# Add the public header files to use the library
set(HYPERSPECTRAL_PUBLIC_HEADERS
    HyperspectralFactory.hpp
)

# Add the public header files to use the library
set(HYPERSPECTRAL_SOURCE_FILES
#
# HySpex packet utilities
#
	hyspex_swir_384_packet_utils.hpp
	hyspex_swir_384_packet_utils.cpp
	hyspex_vnir_3000N_packet_utils.hpp
	hyspex_vnir_3000N_packet_utils.cpp
	

#
# HySpex VNIR 3000N Cameras
#
    HySpexVNIR_3000N_Model_net.hpp
    HySpexVNIR_3000N_Model_net.cpp
    HySpexVNIR_3000N_Model_direct.hpp
    HySpexVNIR_3000N_Model_direct.cpp
    HySpexVNIR_3000N_Model.hpp
    HySpexVNIR_3000N_Model.cpp
    HySpexVNIR_3000N_View.hpp
    HySpexVNIR_3000N_View.cpp

# HySpex VNIR 3000N Controller Code

    HySpexVNIR_3000N_Controller.hpp
    HySpexVNIR_3000N_Controller.cpp
    HySpexVNIR_3000N_ControllerNetDecoder.hpp
    HySpexVNIR_3000N_ControllerNetDecoder.cpp
    HySpexVNIR_3000N_ControllerNetEncoder.hpp
    HySpexVNIR_3000N_ControllerNetEncoder.cpp

# HySpex VNIR 3000N Property Page Code
	HySpexVNIR_3000N_PropertiesNetDecoder.hpp
	HySpexVNIR_3000N_PropertiesNetDecoder.cpp
	HySpexVNIR_3000N_PropertiesNetEncoder.hpp
	HySpexVNIR_3000N_PropertiesNetEncoder.cpp
	HySpexVNIR_3000N_PropertyPage_Remote.hpp
	HySpexVNIR_3000N_PropertyPage_Remote.cpp
	HySpexVNIR_3000N_PropertyPage.hpp
	HySpexVNIR_3000N_PropertyPage.cpp

	
#
# HySpex SWIR 384 Cameras
#
    HySpexSWIR_384_Model_net.hpp
    HySpexSWIR_384_Model_net.cpp
    HySpexSWIR_384_Model_direct.hpp
    HySpexSWIR_384_Model_direct.cpp
    HySpexSWIR_384_Model.hpp
    HySpexSWIR_384_Model.cpp
    HySpexSWIR_384_View.hpp
    HySpexSWIR_384_View.cpp

# HySpex SWIR 384 Controller Code

    HySpexSWIR_384_Controller.hpp
    HySpexSWIR_384_Controller.cpp
    HySpexSWIR_384_ControllerNetDecoder.hpp
    HySpexSWIR_384_ControllerNetDecoder.cpp
    HySpexSWIR_384_ControllerNetEncoder.hpp
    HySpexSWIR_384_ControllerNetEncoder.cpp

# HySpex SWIR 384 Property Page Code
	HySpexSWIR_384_PropertiesNetDecoder.hpp
	HySpexSWIR_384_PropertiesNetDecoder.cpp
	HySpexSWIR_384_PropertiesNetEncoder.hpp
	HySpexSWIR_384_PropertiesNetEncoder.cpp
	HySpexSWIR_384_PropertyPage_Remote.hpp
	HySpexSWIR_384_PropertyPage_Remote.cpp
	HySpexSWIR_384_PropertyPage.hpp
	HySpexSWIR_384_PropertyPage.cpp


#
# General Hyperspectral Camera View-Model-Controller files
#
    HyperspectralView.hpp
    HyperspectralView.cpp
    HyperspectralModel.hpp
    HyperspectralModel.cpp

    HySpexFactory.hpp
    HySpexFactory.cpp

    HyperspectralFactory.cpp
)


# The static library (.lib/.a) for communicating with Hyperspectral Cameras
add_library(hyperspectral_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(hyperspectral_static PROPERTIES OUTPUT_NAME "libHyperspectral")
else ()
	set_target_properties(hyperspectral_static PROPERTIES OUTPUT_NAME "Hyperspectral")
endif (MSVC)

set_target_properties(hyperspectral_static PROPERTIES LANGUAGE CXX)

target_compile_features(hyperspectral_static PRIVATE cxx_std_17)

target_sources(hyperspectral_static
PUBLIC
    ${HYPERSPECTRAL_PUBLIC_HEADERS}
 
PRIVATE
    ${HYPERSPECTRAL_SOURCE_FILES}

	${HYSPEX_SWIR384_PROTO_HDRS}
    ${HYSPEX_SWIR384_PROTO_SRCS}
	hyspex_swir_384_properties.proto

	${HYSPEX_VNIR3000N_PROTO_HDRS}
    ${HYSPEX_VNIR3000N_PROTO_SRCS}
	hyspex_vnir_3000N_properties.proto
)

target_include_directories(hyperspectral_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(hyperspectral_static PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(hyperspectral_static PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})
target_include_directories(hyperspectral_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(hyperspectral_static PUBLIC ${Qt}::Widgets)
target_link_libraries(hyperspectral_static PUBLIC ${Qt}::Network)
target_link_libraries(hyperspectral_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(hyperspectral_static PUBLIC BlockDataFile)
target_link_libraries(hyperspectral_static PRIVATE ${Protobuf_LIBRARIES})

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET hyperspectral_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(hyperspectral_static PROPERTIES FOLDER "Sensors/Hyperspectral")
