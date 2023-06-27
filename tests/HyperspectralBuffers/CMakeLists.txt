
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#
# Tests of the Hyperspectral buffer code
#

add_executable(HyperspectraBufferTests)

set_target_properties(HyperspectraBufferTests PROPERTIES LANGUAGE CXX)

target_compile_features(HyperspectraBufferTests PRIVATE cxx_std_17)

target_sources(HyperspectraBufferTests
PRIVATE
	hyperspectral_buffer_tests.cpp
)

#target_include_directories(HyperspectraBufferTests PRIVATE 
#	$<BUILD_INTERFACE:${LIBOUSTER_INCLUDE_DIR}>
#	$<INSTALL_INTERFACE:include>
#)

#
# Setting MSVC_STATIC_RUNTIME determines which MSVC runtime to use...
if (MSVC_STATIC_RUNTIME)
	# To use MultiThreaded (-MT) and MultiThreadedDebug (-MTd)
	set_property(TARGET HyperspectraBufferTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
	# To use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
	set_property(TARGET HyperspectraBufferTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

target_include_directories(HyperspectraBufferTests PRIVATE ../../src/Sensors)

target_link_libraries(HyperspectraBufferTests PRIVATE Catch2::Catch2WithMain)

message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

set_target_properties(HyperspectraBufferTests PROPERTIES FOLDER "Tests")
