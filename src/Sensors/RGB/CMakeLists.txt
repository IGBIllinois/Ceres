
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(RGB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})


# Add the public header files to use the library
set(RGB_CAMERA_PUBLIC_HEADERS
    RgbTypes.hpp
	RgbCameraFactory.hpp
)

# Add the public header files to use the library
set(RGB_CAMERA_SOURCE_FILES

#
# General RGB Camera View-Model-Controller files
#
    RgbImageWidget.hpp
    RgbImageWidget.cpp
    RgbCameraView.hpp
    RgbCameraView.cpp
    RgbCameraModel.hpp
    RgbCameraModel.cpp
    RgbCameraFactory.cpp
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

target_compile_features(rgb_camera_static PRIVATE cxx_std_20)

target_sources(rgb_camera_static
PUBLIC
    ${RGB_CAMERA_PUBLIC_HEADERS}
 
PRIVATE
    ${RGB_CAMERA_SOURCE_FILES}
)

target_include_directories(rgb_camera_static PRIVATE ${UTILITIES_INCLUDE_DIR})

target_link_libraries(rgb_camera_static PRIVATE cbdf::rgb)
target_link_libraries(rgb_camera_static PUBLIC ${Qt}::Multimedia)
target_link_libraries(rgb_camera_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(rgb_camera_static PUBLIC axis_communications_static)
target_link_libraries(rgb_camera_static PUBLIC lucid_vision_labs_rgb_static)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET rgb_camera_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(rgb_camera_static PROPERTIES FOLDER "Sensors/RGB_Camera")


# Add the Axis Communications WebCam library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/AxisCommunications)

# Add the Lucid Vision Labs RGB Camera library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/LucidVisionLabs)
