
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# source_group("Resources" FILES hokuyo_properties.proto)

# Generate the header/source files needed from the protobuf
# protobuf_generate_cpp(HUKUYO_PROTO_SRCS HUKUYO_PROTO_HDRS DESCRIPTORS PROTO_DESCS hokuyo_properties.proto)


# Add the public header files to use the library
set(HOKUYO_PUBLIC_HEADERS
    HokuyoFactory.hpp
)

# Add the source files used in the library
set(HOKUYO_SOURCE_FILES
#
# Hokuyo packet utilities
#
#	hokuyo_packet_utils.hpp
#	hokuyo_packet_utils.cpp

# Hokuyo Property Page Code
#	HokuyoPropertiesNetDecoder.hpp
#	HokuyoPropertiesNetDecoder.cpp
#	HokuyoPropertiesNetEncoder.hpp
#	HokuyoPropertiesNetEncoder.cpp
#	HokuyoPropertyPage_Remote.hpp
#	HokuyoPropertyPage_Remote.cpp
#   HokuyoPropertyPage_Local.hpp
#   HokuyoPropertyPage_Local.cpp
#	HokuyoPropertyPage.hpp
#	HokuyoPropertyPage.cpp

#    HokuyoExperimentStates.hpp
#    HokuyoExperimentStates.cpp

# Hokuyo Controller Code

#	HokuyoControllerNetDecoder.hpp
#	HokuyoControllerNetDecoder.cpp
#	HokuyoControllerNetEncoder.hpp
#	HokuyoControllerNetEncoder.cpp
#    HokuyoController.hpp
#    HokuyoController.cpp

# Network stream from sensor
#    HokuyoCmdStream.hpp
#    HokuyoCmdStream.cpp
#    HokuyoDataStream.hpp
#    HokuyoDataStream.cpp
#    HokuyoAsyncCommands.hpp
#    HokuyoAsyncCommands.cpp
    HokuyoModel_UST10LX.hpp
    HokuyoModel_UST10LX.cpp

#
# Base HOKUYO LiDAR files
#
    HokuyoView.hpp
    HokuyoView.cpp
    HokuyoModel.hpp
    HokuyoModel.cpp
    HokuyoFactory.cpp

)


# The static library (.lib/.a) for incorporating OUSTER LiDAR data in Ceres
add_library(hokuyo_sensor_static STATIC)


if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(hokuyo_sensor_static PROPERTIES OUTPUT_NAME "libHokuyoSensor")
else ()
	set_target_properties(hokuyo_sensor_static PROPERTIES OUTPUT_NAME "HokuyoSensor")
endif (MSVC)

set_target_properties(hokuyo_sensor_static PROPERTIES LANGUAGE CXX)

target_compile_features(hokuyo_sensor_static PRIVATE cxx_std_17)

target_sources(hokuyo_sensor_static

PUBLIC
    ${HOKUYO_PUBLIC_HEADERS}
 
PRIVATE
    ${HOKUYO_SOURCE_FILES}

#    ${HOKUYO_PROTO_HDRS}
#	${HOKUYO_PROTO_SRCS}
#	hokuyo_properties.proto
)

target_include_directories(hokuyo_sensor_static PRIVATE ${UTILITIES_INCLUDE_DIR})
target_include_directories(hokuyo_sensor_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

#target_link_libraries(hokuyo_sensor_static PRIVATE cbdf::lidar)

target_link_libraries(hokuyo_sensor_static PUBLIC ${Qt}::Widgets)
target_link_libraries(hokuyo_sensor_static PUBLIC ${Qt}::Network)
target_link_libraries(hokuyo_sensor_static PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(hokuyo_sensor_static PRIVATE ouster::ouster_static)
#target_link_libraries(hokuyo_sensor_static INTERFACE ouster::OusterVisualization)
target_link_libraries(hokuyo_sensor_static PRIVATE ${Protobuf_LIBRARIES})


#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET hokuyo_sensor_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(hokuyo_sensor_static PROPERTIES FOLDER "Sensors/Lidar/Hokuyo")
