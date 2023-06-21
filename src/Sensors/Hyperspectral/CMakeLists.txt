
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Add the public header files to use the library
set(HYPERSPECTRAL_PUBLIC_HEADERS
    HyperspectralFactory.hpp
)

# Add the public header files to use the library
set(HYPERSPECTRAL_SOURCE_FILES
#
# General Hyperspectral Camera View-Model-Controller files
#
    HyperspectralView.hpp
    HyperspectralView.cpp
    HyperspectralModel.hpp
    HyperspectralModel.cpp

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
)

target_link_libraries(hyperspectral_static PRIVATE cbdf::hyperspectral)

target_link_libraries(hyperspectral_static PUBLIC ${Qt}::Widgets)
target_link_libraries(hyperspectral_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(hyperspectral_static PUBLIC hyspex_static)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET hyperspectral_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(hyperspectral_static PROPERTIES FOLDER "Sensors/Hyperspectral")


# Add the HySpex Hyperspectral library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/HySpex)
