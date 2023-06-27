
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#
# Tests of the Block File code
#

add_executable(HyperspectraBufferTests)

set_target_properties(HyperspectraBufferTests PROPERTIES LANGUAGE CXX)

target_compile_features(HyperspectraBufferTests PRIVATE cxx_std_17)

target_sources(HyperspectraBufferTests
PRIVATE
)

target_include_directories(HyperspectraBufferTests PRIVATE 
	$<BUILD_INTERFACE:${LIBOUSTER_INCLUDE_DIR}>
	$<INSTALL_INTERFACE:include>
)

#
# Setting MSVC_STATIC_RUNTIME determines which MSVC runtime to use...
if (MSVC_STATIC_RUNTIME)
	# To use MultiThreaded (-MT) and MultiThreadedDebug (-MTd)
	set_property(TARGET HyperspectraBufferTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
	# To use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
	set_property(TARGET HyperspectraBufferTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

#target_include_directories(HyperspectraBufferTests PRIVATE ../../src/BlockDataFile)

#target_link_libraries(HyperspectraBufferTests PRIVATE block_data_file_static)
target_link_libraries(HyperspectraBufferTests PRIVATE cbdf::cbdf cbdf::info cbdf::ctrl cbdf::sensors cbdf::weather)
target_link_libraries(HyperspectraBufferTests PRIVATE Catch2::Catch2WithMain)

# For OUSTER serializer/parsing tests
target_link_libraries(HyperspectraBufferTests PRIVATE ouster::ouster_static)

# For SSNX GPS serializer/parsing tests
target_link_libraries(HyperspectraBufferTests PUBLIC ssnx::ssnx_static)

message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

set_target_properties(HyperspectraBufferTests PROPERTIES FOLDER "Tests")
