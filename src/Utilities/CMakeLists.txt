

add_library(Utilities STATIC)

#set_target_properties(Utilities PROPERTIES LANGUAGE C CXX)

target_compile_features(Utilities PRIVATE cxx_std_17)

target_sources(Utilities
PUBLIC

	Constants.hpp
	StringUtils.hpp
	Utilities.hpp
	ColorGradient.hpp
	Timers.hpp
	QIndicator.hpp
	QLogWidget.hpp
	
	net_buffer.hpp
	packet_utils.hpp
	net_packet_encoder.hpp
	net_packet_decoder.hpp


PRIVATE

	ColorGradient.cpp
	Timers.cpp
	QIndicator.cpp
	QLogWidget.cpp
	StringUtils.cpp
	
	net_buffer.cpp
	packet_utils.cpp
	net_packet_encoder.cpp
	net_packet_decoder.cpp
)

target_link_libraries(Utilities PRIVATE ${Qt}::Widgets)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET Utilities PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
