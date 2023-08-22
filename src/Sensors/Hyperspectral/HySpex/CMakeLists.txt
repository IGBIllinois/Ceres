
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Resources" FILES hyspex_properties.proto)

# Generate the header/source files needed from the protobuf
protobuf_generate_cpp(HYSPEX_PROTO_SRCS HYSPEX_PROTO_HDRS DESCRIPTORS PROTO_DESCS hyspex_properties.proto)

# Add the public header files to use the library
set(HYSPEX_PUBLIC_HEADERS
	HySpexFactory.hpp
)

# Add the public header files to use the library
set(HYSPEX_SOURCE_FILES
#
# HySpex packet utilities
#
	hyspex_packet_utils.hpp
	hyspex_packet_utils.cpp

#
# HySpex VNIR 3000N Cameras
#
    HySpexVNIR_3000N_Model_net.hpp
    HySpexVNIR_3000N_Model_net.cpp
    HySpexVNIR_3000N_Model_direct.hpp
    HySpexVNIR_3000N_Model_direct.cpp
    HySpexVNIR_3000N_Model_simulation.hpp
    HySpexVNIR_3000N_Model_simulation.cpp
    HySpexVNIR_3000N_Model.hpp
    HySpexVNIR_3000N_Model.cpp
    HySpexVNIR_3000N_View.hpp
    HySpexVNIR_3000N_View.cpp
    HySpexVNIR_3000N_StatusView.hpp
    HySpexVNIR_3000N_StatusView.cpp

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
    HySpexSWIR_384_StatusView.hpp
    HySpexSWIR_384_StatusView.cpp

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
# HySpex Experiment State Files
#
	HySpexExperimentStates.hpp
	HySpexExperimentStates.cpp
	
#
# HySpex Cameras General Files
#
	HySpexDataTypes.hpp
	HySpexCamera_PropertyPage.hpp
	HySpexCamera_PropertyPage.cpp
	HySpexCamera_PropertiesNetDecoder.hpp
	HySpexCamera_PropertiesNetDecoder.cpp
	HySpexCamera_PropertiesNetEncoder.hpp
	HySpexCamera_PropertiesNetEncoder.cpp
    HySpexCamera_Controller.hpp
    HySpexCamera_Controller.cpp
	HySpexCamera_ControllerNetDecoder.hpp
	HySpexCamera_ControllerNetDecoder.cpp
	HySpexCamera_ControllerNetEncoder.hpp
	HySpexCamera_ControllerNetEncoder.cpp
    HySpexCamera_Model.hpp
    HySpexCamera_Model.cpp
    HySpexCamera_StatusView.hpp
    HySpexCamera_StatusView.cpp
	hyspex_utils.hpp
	hyspex_utils.cpp
    HySpexFactory.cpp
)


#
# We skip these header files as they are public and MOC will be used 
# in the main application
#set_property(SOURCE ExperimentCtrlView.hpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)


# The static library (.lib/.a) for communicating with HySpex Cameras
add_library(hyspex_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(hyspex_static PROPERTIES OUTPUT_NAME "libHySpex")
else ()
	set_target_properties(hyspex_static PROPERTIES OUTPUT_NAME "HySpex")
endif (MSVC)

set_target_properties(hyspex_static PROPERTIES LANGUAGE CXX)

target_compile_features(hyspex_static PRIVATE cxx_std_17)

target_sources(hyspex_static
PUBLIC
    ${HYSPEX_PUBLIC_HEADERS}
 
PRIVATE
    ${HYSPEX_SOURCE_FILES}

	${HYSPEX_PROTO_HDRS}
    ${HYSPEX_PROTO_SRCS}
	hyspex_properties.proto
)

target_include_directories(hyspex_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(hyspex_static PRIVATE ${UTILITIES_INCLUDE_DIR})

target_include_directories(hyspex_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(hyspex_static PUBLIC ${Qt}::Widgets)
target_link_libraries(hyspex_static PUBLIC ${Qt}::Network)
target_link_libraries(hyspex_static PRIVATE QCustomPlot::qcustomplot_static)
target_link_libraries(hyspex_static PRIVATE ExperimentStateMachine)
target_link_libraries(hyspex_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(hyspex_static PRIVATE ${Protobuf_LIBRARIES})
target_link_libraries(hyspex_static PRIVATE HySpexConnect::HySpexConnect_static)
target_link_libraries(hyspex_static PRIVATE HySpex::HySpex)
target_link_libraries(hyspex_static PRIVATE Spinnaker::Spinnaker)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET hyspex_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(hyspex_static PROPERTIES FOLDER "Sensors/Hyperspectral/HySpex")
