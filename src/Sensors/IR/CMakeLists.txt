
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(IR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Add the public header files to use the library
set(IR_CAMERA_PUBLIC_HEADERS
    IrCameraFactory.hpp
)

# Add the public header files to use the library
set(IR_CAMERA_SOURCE_FILES

#
# General IR Camera View-Model-Controller files
#
    IrCameraView.hpp
    IrCameraView.cpp
    IrCameraModel.hpp
    IrCameraModel.cpp
    IrCameraFactory.cpp
)


# The static library (.lib/.a) for communicating with Multispectral Cameras
add_library(ir_camera_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(ir_camera_static PROPERTIES OUTPUT_NAME "libIrCamera")
else ()
	set_target_properties(ir_camera_static PROPERTIES OUTPUT_NAME "IrCamera")
endif (MSVC)

set_target_properties(ir_camera_static PROPERTIES LANGUAGE CXX)

target_compile_features(ir_camera_static PRIVATE cxx_std_20)

target_sources(ir_camera_static
PUBLIC
    ${IR_CAMERA_PUBLIC_HEADERS}
 
PRIVATE
    ${IR_CAMERA_SOURCE_FILES}
)

target_include_directories(ir_camera_static PRIVATE ${COMMON_INCLUDE_DIR})

#target_include_directories(ir_camera_static PRIVATE cbdf::ir)

target_link_libraries(ir_camera_static PUBLIC ${Qt}::Widgets)
target_link_libraries(ir_camera_static PUBLIC ${Qt}::Network)
target_link_libraries(ir_camera_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(ir_camera_static PUBLIC teledyne_flir_static)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET ir_camera_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(ir_camera_static PROPERTIES FOLDER "Sensors/IR_Camera")


# Add the Teledyne FLIR thermal camera library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/TeledyneFLIR)
