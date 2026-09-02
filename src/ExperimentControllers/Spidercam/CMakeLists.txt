
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(resource_files
  spidercam_resources.qrc
)

source_group("Resources" FILES
  ${resource_files}
)

if (${Qt} STREQUAL "Qt6")
	qt6_add_resources(SPIDERCAM_RESOURCES spidercam_resources.qrc OPTIONS "--verbose")
else()
	qt5_add_resources(SPIDERCAM_RESOURCES spidercam_resources.qrc OPTIONS "--verbose")
endif()


# Add the public header files to use the library
set(SPIDERCAM_CTRL_PUBLIC_HEADERS

    SpidercamCtrlFactory.hpp
)

# Add the source files used in the library
set(SPIDERCAM_CTRL_SOURCE_FILES

    SpidercamCtrlIDs.hpp

#
# Spidercam Network files
#
    SpidercamModel_net.hpp
    SpidercamModel_net.cpp

#
# Spidercam Simulation files
#
    SpidercamCtrlPanel.hpp
    SpidercamCtrlPanel.cpp
    SpidercamModel_sim.hpp
    SpidercamModel_sim.cpp

#
# Spidercam files
#
    SpidercamDollyStatus.hpp
    SpidercamDollyStatus.cpp
    SpidercamToolbar.hpp
    SpidercamToolbar.cpp
    SpidercamScanArea.hpp
    SpidercamScanArea.cpp
    SpidercamView.hpp
    SpidercamView.cpp
    SpidercamModel.hpp
    SpidercamModel.cpp
    SpidercamCtrlProxies.hpp
    SpidercamCtrl.hpp
    SpidercamCtrl.cpp
    SpidercamUtils.hpp
    SpidercamUtils.cpp

    SpidercamCtrlFactory.cpp

#
# Spidercam State Machine files
#
    SpidercamExperimentStates.hpp
    SpidercamExperimentStates.cpp

#
# Aerial Compensation States for the experiment state machine
# 
# The spidercam dolly droops in the middle of the field.  The
# aerial compensation states provide the necessary data to
# compensate for the droop
#
    AerialCompensationDlg.hpp
    AerialCompensationDlg.cpp
    AerialCompensationStates.hpp
    AerialCompensationStates.cpp
	
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

target_compile_features(SpidercamCtrl_static PRIVATE cxx_std_20)

target_sources(SpidercamCtrl_static

PUBLIC
    ${SPIDERCAM_CTRL_PUBLIC_HEADERS}
 
PRIVATE
    ${SPIDERCAM_RESOURCES}
    ${SPIDERCAM_CTRL_SOURCE_FILES}
)

target_include_directories(SpidercamCtrl_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(SpidercamCtrl_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(SpidercamCtrl_static PUBLIC cbdf::ctrl)
target_link_libraries(SpidercamCtrl_static PRIVATE cbdf::info)
target_link_libraries(SpidercamCtrl_static PUBLIC ${Qt}::Widgets)
target_link_libraries(SpidercamCtrl_static PUBLIC ${Qt}::Network)
target_link_libraries(SpidercamCtrl_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(SpidercamCtrl_static PRIVATE spidercam_connect::spidercam_connect_static)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET SpidercamCtrl_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(SpidercamCtrl_static PROPERTIES FOLDER "ExperimentControllers/Spidercam")
