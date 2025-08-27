
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#
# Tests of the HySpex "focus" code
#

add_executable(HySpexFocusTests)

set_target_properties(HySpexFocusTests PROPERTIES LANGUAGE CXX)

target_compile_features(HySpexFocusTests PRIVATE cxx_std_20)

target_sources(HySpexFocusTests
PRIVATE
	hyspex_focus_tests.cpp
)

#target_include_directories(HySpexFocusTests PRIVATE 
#	$<BUILD_INTERFACE:${LIBOUSTER_INCLUDE_DIR}>
#	$<INSTALL_INTERFACE:include>
#)

#
# Setting MSVC_STATIC_RUNTIME determines which MSVC runtime to use...
if (MSVC_STATIC_RUNTIME)
	# To use MultiThreaded (-MT) and MultiThreadedDebug (-MTd)
	set_property(TARGET HySpexFocusTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
	# To use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
	set_property(TARGET HySpexFocusTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

target_include_directories(HySpexFocusTests PRIVATE ../../src/Sensors)
target_include_directories(HySpexFocusTests PRIVATE ../../src/Utilities)

target_link_libraries(HySpexFocusTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(HySpexFocusTests PRIVATE hyspex_connect::hyspex_connect_minimal_static)

message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

set_target_properties(HySpexFocusTests PROPERTIES FOLDER "Tests")
