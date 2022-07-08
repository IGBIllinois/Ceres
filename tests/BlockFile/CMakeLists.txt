
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#
# Tests of the Block File code
#

add_executable(BlockFileTests)

set_target_properties(BlockFileTests PROPERTIES LANGUAGE CXX)

target_compile_features(BlockFileTests PRIVATE cxx_std_17)

target_sources(BlockFileTests
PRIVATE
	${OUSTER_PUBLIC_HEADERS}

	block_file.cpp
	pvt_file.cpp
	ouster_file.cpp
	ssnx_file.cpp
)

target_include_directories(BlockFileTests PRIVATE 
	$<BUILD_INTERFACE:${LIBOUSTER_INCLUDE_DIR}>
	$<INSTALL_INTERFACE:include>
)

#
# Setting MSVC_STATIC_RUNTIME determines which MSVC runtime to use...
if (MSVC_STATIC_RUNTIME)
	# To use MultiThreaded (-MT) and MultiThreadedDebug (-MTd)
	set_property(TARGET BlockFileTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
	# To use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
	set_property(TARGET BlockFileTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

target_include_directories(BlockFileTests PRIVATE ../../src/BlockDataFile)

target_link_libraries(BlockFileTests PRIVATE BlockDataFile)
target_link_libraries(BlockFileTests PRIVATE Catch2::Catch2WithMain)

# For OUSTER serializer/parsing tests
target_link_libraries(BlockFileTests PRIVATE ouster::ouster_static)

# For SSNX GPS serializer/parsing tests
target_link_libraries(gps_static PUBLIC ssnx::ssnx_static)

message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

set_target_properties(BlockFileTests PROPERTIES FOLDER "Tests")
