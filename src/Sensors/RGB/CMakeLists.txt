
# Add the public header files to use the library
set(RGB_CAMERA_PUBLIC_HEADERS
)

# Add the public header files to use the library
set(RGB_CAMERA_SOURCE_FILES

#
# General RGB Camera View-Model-Controller files
#
    RgbCameraView.hpp
    RgbCameraView.cpp
    RgbCameraModel.hpp
    RgbCameraModel.cpp
)


# The static library (.lib/.a) for communicating with Multispectral Cameras
add_library(rgb_camera_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(rgb_camera_static PROPERTIES OUTPUT_NAME "libRgbCamera")
else ()
	set_target_properties(rgb_camera_static PROPERTIES OUTPUT_NAME "RgbCamera")
endif (MSVC)

set_target_properties(rgb_camera_static PROPERTIES LANGUAGE CXX)

target_compile_features(rgb_camera_static PRIVATE cxx_std_17)

target_sources(rgb_camera_static
PUBLIC
    ${RGB_CAMERA_PUBLIC_HEADERS}
 
PRIVATE
    ${RGB_CAMERA_SOURCE_FILES}
)

#target_include_directories(rgb_camera_static PRIVATE BlockDataFile)

target_link_libraries(rgb_camera_static PUBLIC ${Qt}::Widgets)
target_link_libraries(rgb_camera_static PUBLIC ${Qt}::Network)
target_link_libraries(rgb_camera_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(rgb_camera_static PUBLIC BlockDataFile)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET rgb_camera_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(rgb_camera_static PROPERTIES FOLDER "Sensors/RGB_Camera")
