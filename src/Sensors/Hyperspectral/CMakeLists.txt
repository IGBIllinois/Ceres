
# Add the public header files to use the library
set(HYPERSPECTRAL_PUBLIC_HEADERS
    HySpexFactory.hpp
)

# Add the public header files to use the library
set(HYPERSPECTRAL_SOURCE_FILES
#
# HySpex Cameras
#
    HySpexVNIR_3000N_Model_net.hpp
    HySpexVNIR_3000N_Model_net.cpp
    HySpexVNIR_3000N_Model_direct.hpp
    HySpexVNIR_3000N_Model_direct.cpp
    HySpexVNIR_3000N_Model.hpp
    HySpexVNIR_3000N_Model.cpp
    HySpexVNIR_3000N_View.hpp
    HySpexVNIR_3000N_View.cpp
    HySpexSWIR_384_Model_net.hpp
    HySpexSWIR_384_Model_net.cpp
    HySpexSWIR_384_Model_direct.hpp
    HySpexSWIR_384_Model_direct.cpp
    HySpexSWIR_384_Model.hpp
    HySpexSWIR_384_Model.cpp
    HySpexSWIR_384_View.hpp
    HySpexSWIR_384_View.cpp

    HySpexFactory.cpp

#
# General Hyperspectral Camera View-Model-Controller files
#
    HyperspectralView.hpp
    HyperspectralView.cpp
    HyperspectralModel.hpp
    HyperspectralModel.cpp
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

target_include_directories(hyperspectral_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(hyperspectral_static PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})

target_link_libraries(hyperspectral_static PUBLIC ${Qt}::Widgets)
target_link_libraries(hyperspectral_static PUBLIC ${Qt}::Network)
target_link_libraries(hyperspectral_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(hyperspectral_static PUBLIC BlockDataFile)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET hyperspectral_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(hyperspectral_static PROPERTIES FOLDER "Sensors/Hyperspectral")
