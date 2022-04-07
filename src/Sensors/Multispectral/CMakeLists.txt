
# Add the public header files to use the library
set(MULTISPECTRAL_PUBLIC_HEADERS
)

# Add the public header files to use the library
set(MULTISPECTRAL_SOURCE_FILES

#
# General Multispectral Camera View-Model-Controller files
#
    MultispectralView.hpp
    MultispectralView.cpp
    MultispectralModel.hpp
    MultispectralModel.cpp
)


# The static library (.lib/.a) for communicating with Multispectral Cameras
add_library(multispectral_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(multispectral_static PROPERTIES OUTPUT_NAME "libMultispectral")
else ()
	set_target_properties(multispectral_static PROPERTIES OUTPUT_NAME "Multispectral")
endif (MSVC)

set_target_properties(multispectral_static PROPERTIES LANGUAGE CXX)

target_compile_features(multispectral_static PRIVATE cxx_std_17)

target_sources(multispectral_static
PUBLIC
    ${MULTISPECTRAL_PUBLIC_HEADERS}
 
PRIVATE
    ${MULTISPECTRAL_SOURCE_FILES}
)

#target_include_directories(multispectral_static PRIVATE BlockDataFile)

target_link_libraries(multispectral_static PUBLIC ${Qt}::Widgets)
target_link_libraries(multispectral_static PUBLIC ${Qt}::Network)
target_link_libraries(multispectral_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(multispectral_static PUBLIC BlockDataFile)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET multispectral_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(multispectral_static PROPERTIES FOLDER "Sensors/Multispectral")
