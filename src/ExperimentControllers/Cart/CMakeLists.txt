
# Add the public header files to use the library
set(CART_CTRL_PUBLIC_HEADERS

    CartCtrlFactory.hpp
)

# Add the source files used in the library
set(CART_CTRL_SOURCE_FILES

    CartCtrlIDs.hpp

#
# Cart Network files
#
#    SpidercamModel_net.hpp
#    SpidercamModel_net.cpp

#
# Cart Simulation files
#
#    SpidercamModel_sim.hpp
#    SpidercamModel_sim.cpp

#
# Cart files
#
#    SpidercamDollyStatus.hpp
#    SpidercamDollyStatus.cpp
#    SpidercamToolbar.hpp
#    SpidercamToolbar.cpp
#    SpidercamScanArea.hpp
#    SpidercamScanArea.cpp
#    SpidercamView.hpp
#    SpidercamView.cpp
#    SpidercamModel.hpp
#    SpidercamModel.cpp
#    SpidercamCtrlProxies.hpp
#    SpidercamCtrl.hpp
#    SpidercamCtrl.cpp
#    SpidercamUtils.hpp
#    SpidercamUtils.cpp

    CartCtrlFactory.cpp

#
# Cart State Machine files
#
#    SpidercamExperimentStates.hpp
#    SpidercamExperimentStates.cpp
)


# The static library (.lib/.a) for incorporating spidercam control in Ceres
add_library(CartCtrl_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(CartCtrl_static PROPERTIES OUTPUT_NAME "libCartCtrl")
else ()
	set_target_properties(CartCtrl_static PROPERTIES OUTPUT_NAME "CartCtrl")
endif (MSVC)

set_target_properties(CartCtrl_static PROPERTIES LANGUAGE CXX)

target_compile_features(CartCtrl_static PRIVATE cxx_std_20)

target_sources(CartCtrl_static

PUBLIC
    ${CART_CTRL_PUBLIC_HEADERS}
 
PRIVATE
    ${CART_CTRL_SOURCE_FILES}
)

target_include_directories(CartCtrl_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(CartCtrl_static PRIVATE ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(CartCtrl_static PUBLIC cbdf::ctrl)
target_link_libraries(CartCtrl_static PRIVATE cbdf::info)
target_link_libraries(CartCtrl_static PUBLIC ${Qt}::Widgets)
target_link_libraries(CartCtrl_static PUBLIC ${Qt}::Network)
target_link_libraries(CartCtrl_static PRIVATE nlohmann_json::nlohmann_json)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET CartCtrl_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(CartCtrl_static PROPERTIES FOLDER "ExperimentControllers/Cart")
