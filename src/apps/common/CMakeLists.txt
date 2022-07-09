
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


add_library(common STATIC)

#set_target_properties(Sensors PROPERTIES LANGUAGE C CXX)

target_compile_features(common PRIVATE cxx_std_17)

source_group("Resources" FILES
	ceres_remote_client.proto
	spidercam_data.proto
	weather_data.proto
)

# Generate the header/source files needed from the protobuf
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS DESCRIPTORS PROTO_DESCS ceres_remote_client.proto spidercam_data.proto weather_data.proto)


target_sources(common

PRIVATE
#
# General Application files
#
    DataModel.hpp
    DataModel.cpp
	
    DataThread.hpp
    DataThread.cpp
	
    TimestampProvider.hpp
    TimestampProvider.cpp
	
    CeresSplashScreen.hpp
    CeresSplashScreen.cpp
	
	net_buffer.hpp
	net_buffer.cpp
	
	packet_utils.hpp
	packet_utils.cpp

	ceres_net_decoder.hpp
	ceres_net_decoder.cpp
	ceres_net_encoder.hpp
	ceres_net_encoder.cpp

	ceres_remote_client_net_decoder.hpp
	ceres_remote_client_net_decoder.cpp
	ceres_remote_client_net_encoder.hpp
	ceres_remote_client_net_encoder.cpp

	net_packet_decoder.hpp
	net_packet_decoder.cpp
	net_packet_encoder.hpp
	net_packet_encoder.cpp

	${PROTO_HDRS}
	${PROTO_SRCS}
	ceres_remote_client.proto
	spidercam_data.proto
	weather_data.proto
)

#
# We skip these header files as they are public and MOC will be used 
# in the main application
#set_property(SOURCE CeresSplashScreen.hpp PROPERTY SKIP_AUTOMOC ON)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET common PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_include_directories(common PRIVATE ${SENSORS_INCLUDE_DIR})

target_link_libraries(common PRIVATE ${Qt}::Widgets)
target_link_libraries(common PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(common PRIVATE ${Protobuf_LIBRARIES})

set_target_properties(common PROPERTIES FOLDER "Apps")



#set_target_properties(Sensors PROPERTIES FOLDER "Sensors")



