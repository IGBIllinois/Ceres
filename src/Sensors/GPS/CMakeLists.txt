
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Resources" FILES gps_properties.proto)

# Generate the header/source files needed from the protobuf
protobuf_generate_cpp(GPS_PROTO_SRCS GPS_PROTO_HDRS DESCRIPTORS PROTO_DESCS gps_properties.proto)


# Add the public header files to use the library
set(GPS_PUBLIC_HEADERS
    GpsFactory.hpp
)

# Add the source files used in the library
set(GPS_SOURCE_FILES
#
# GPS packet utilities
#
	gps_packet_utils.hpp
	gps_packet_utils.cpp

#
# General GPS View-Model-Controller files
#
	GpsTypes.hpp
    GpsUtils.hpp
    GpsUtils.cpp
    GpsView.hpp
    GpsView.cpp
    GpsModel.hpp
    GpsModel.cpp
    GpsFactory.cpp

# General GPS Controller Code

    GpsController.hpp
    GpsController.cpp
    GpsControllerNetDecoder.hpp
    GpsControllerNetDecoder.cpp
    GpsControllerNetEncoder.hpp
    GpsControllerNetEncoder.cpp


# General GPS Property Page Code
	GpsPropertiesNetDecoder.hpp
	GpsPropertiesNetDecoder.cpp
	GpsPropertiesNetEncoder.hpp
	GpsPropertiesNetEncoder.cpp
#    GpsPropertyPage_Remote.hpp
#	GpsPropertyPage_Remote.cpp
#	GpsPropertyPage.hpp
#	GpsPropertyPage.cpp

#
# Gps Experiment State Files
#
    GpsExperimentStates_Local.hpp
    GpsExperimentStates_Local.cpp
    GpsExperimentStates_Remote.hpp
    GpsExperimentStates_Remote.cpp
)


#
# We skip these header files as they are public and MOC will be used 
# in the main application
#set_property(SOURCE ExperimentCtrlView.hpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)


# The static library (.lib/.a) for incorporating GPS data in Ceres
add_library(gps_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(gps_static PROPERTIES OUTPUT_NAME "libGPS")
else ()
	set_target_properties(gps_static PROPERTIES OUTPUT_NAME "GPS")
endif (MSVC)

set_target_properties(gps_static PROPERTIES LANGUAGE CXX)

target_compile_features(gps_static PRIVATE cxx_std_20)

target_sources(gps_static

PUBLIC
    ${GPS_PUBLIC_HEADERS}
 
PRIVATE
    ${GPS_SOURCE_FILES}

	${GPS_PROTO_HDRS}
    ${GPS_PROTO_SRCS}
	gps_properties.proto
)

target_include_directories(gps_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(gps_static PRIVATE ${UTILITIES_INCLUDE_DIR})

target_include_directories(gps_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(gps_static PUBLIC ${Qt}::Network ${Qt}::Widgets)
target_link_libraries(gps_static PRIVATE ExperimentStateMachine)
target_link_libraries(gps_static PRIVATE cbdf::gps)
target_link_libraries(gps_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(gps_static PRIVATE septentrio_static)
target_link_libraries(gps_static PUBLIC ${Protobuf_LIBRARIES})
target_link_libraries(gps_static PUBLIC utf8_range::utf8_range)
target_link_libraries(gps_static PUBLIC
    absl::absl_check
    absl::absl_log
    absl::algorithm
    absl::base
    absl::bind_front
    absl::bits
    absl::btree
    absl::cleanup
    absl::cord
    absl::core_headers
    absl::debugging
    absl::die_if_null
    absl::dynamic_annotations
    absl::flags
    absl::flat_hash_map
    absl::flat_hash_set
    absl::function_ref
    absl::hash
    absl::layout
    absl::log_initialize
    absl::log_globals
    absl::log_severity
    absl::memory
    absl::node_hash_map
    absl::node_hash_set
    absl::optional
    absl::random_distributions
    absl::random_random
    absl::span
    absl::status
    absl::statusor
    absl::strings
    absl::synchronization
    absl::time
    absl::utility
)


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET gps_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(gps_static PROPERTIES FOLDER "Sensors/GPS")


# Add the Septentrio GPS library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Septentrio)
