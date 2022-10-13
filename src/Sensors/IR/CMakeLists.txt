
# Add the public header files to use the library
set(IR_CAMERA_PUBLIC_HEADERS
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

target_compile_features(ir_camera_static PRIVATE cxx_std_17)

target_sources(ir_camera_static
PUBLIC
    ${IR_CAMERA_PUBLIC_HEADERS}
 
PRIVATE
    ${IR_CAMERA_SOURCE_FILES}
)

target_include_directories(ir_camera_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(ir_camera_static PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})

target_link_libraries(ir_camera_static PUBLIC ${Qt}::Widgets)
target_link_libraries(ir_camera_static PUBLIC ${Qt}::Network)
target_link_libraries(ir_camera_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(ir_camera_static PUBLIC block_data_file_static)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET ir_camera_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(ir_camera_static PROPERTIES FOLDER "Sensors/IR_Camera")
