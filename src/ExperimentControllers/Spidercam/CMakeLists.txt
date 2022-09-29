
# Add the public header files to use the library
set(SPIDERCAM_CTRL_PUBLIC_HEADERS

    SpidercamCtrlFactory.hpp
)

# Add the source files used in the library
set(SPIDERCAM_CTRL_SOURCE_FILES
#
# Spidercam files
#
    SpidercamDollyStatus.hpp
    SpidercamDollyStatus.cpp
    SpidercamToolbar.hpp
    SpidercamToolbar.cpp
    SpidercamExperimentStates.hpp
    SpidercamExperimentStates.cpp
    SpidercamScanArea.hpp
    SpidercamScanArea.cpp
    SpidercamView.hpp
    SpidercamView.cpp
    SpidercamModel.hpp
    SpidercamModel.cpp
    SpidercamCtrl.hpp
    SpidercamCtrl.cpp
    SpidercamUtils.hpp
    SpidercamUtils.cpp

    SpidercamCtrlFactory.cpp
)


# The static library (.lib/.a) for incorporating spidercam control in Ceres
add_library(SpidercamCtrl_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(SpidercamCtrl_static PROPERTIES OUTPUT_NAME "libSpidercamCtrl")
else ()
	set_target_properties(SpidercamCtrl_static PROPERTIES OUTPUT_NAME "SpidercamCtrl")
endif (MSVC)

set_target_properties(SpidercamCtrl_static PROPERTIES LANGUAGE CXX)

target_compile_features(SpidercamCtrl_static PRIVATE cxx_std_17)

target_sources(SpidercamCtrl_static

PUBLIC
    ${SPIDERCAM_CTRL_PUBLIC_HEADERS}
 
PRIVATE
    ${SPIDERCAM_CTRL_SOURCE_FILES}
)

target_include_directories(SpidercamCtrl_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(SpidercamCtrl_static PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})
target_include_directories(SpidercamCtrl_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(SpidercamCtrl_static PUBLIC ${Qt}::Widgets)
target_link_libraries(SpidercamCtrl_static PUBLIC ${Qt}::Network)
target_link_libraries(SpidercamCtrl_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(SpidercamCtrl_static PUBLIC BlockDataFile)
target_link_libraries(SpidercamCtrl_static PRIVATE spidercam::scConnect_static)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET SpidercamCtrl_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(SpidercamCtrl_static PROPERTIES FOLDER "ExperimentControllers/Spidercam")
