
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(RGB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})


# Add the public header files to use the library
set(TOF_CAMERA_PUBLIC_HEADERS
    TofCameraFactory.hpp
)

# Add the public header files to use the library
set(TOF_CAMERA_SOURCE_FILES

#
# General Time-Of-Flight Camera View-Model-Controller files
#
    TofCameraView.hpp
    TofCameraView.cpp
    TofCameraModel.hpp
    TofCameraModel.cpp
    TofCameraFactory.cpp
)


# The static library (.lib/.a) for communicating with Time-of-Flight Cameras
add_library(tof_camera_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(tof_camera_static PROPERTIES OUTPUT_NAME "libTofCamera")
else ()
	set_target_properties(tof_camera_static PROPERTIES OUTPUT_NAME "TofCamera")
endif (MSVC)

set_target_properties(tof_camera_static PROPERTIES LANGUAGE CXX)

target_compile_features(tof_camera_static PRIVATE cxx_std_20)

target_sources(tof_camera_static
PUBLIC
    ${TOF_CAMERA_PUBLIC_HEADERS}
 
PRIVATE
    ${TOF_CAMERA_SOURCE_FILES}
)

target_include_directories(tof_camera_static PRIVATE ${COMMON_INCLUDE_DIR})

#target_include_directories(tof_camera_static PRIVATE cbdf::tof)

target_link_libraries(tof_camera_static PUBLIC ${Qt}::Widgets)
target_link_libraries(tof_camera_static PUBLIC ${Qt}::Network)
target_link_libraries(tof_camera_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(tof_camera_static PUBLIC lucid_vision_labs_static)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET tof_camera_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(tof_camera_static PROPERTIES FOLDER "Sensors/TOF_Camera")


# Add the Lucid Vision Labs time-of-flight library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/LucidVisionLabs)

