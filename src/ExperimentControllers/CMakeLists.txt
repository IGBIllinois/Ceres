
set(CMAKE_AUTOMOC ON)


add_library(ExperimentControllers STATIC)

set_target_properties(ExperimentControllers PROPERTIES LANGUAGE CXX)

target_compile_features(ExperimentControllers PRIVATE cxx_std_20)

target_sources(ExperimentControllers
PUBLIC
#
# Base Headers for all Experiment Controller classes
#
    ExperimentCtrlView.hpp
    ExperimentCtrlModel.hpp
 
PRIVATE
#
# A dummy Experiment Controller for sensor testing
#
    DummyCtrlView.hpp
    DummyCtrlView.cpp
    DummyCtrlModel.hpp
    DummyCtrlModel.cpp
    DummyCtrlFactory.hpp
    DummyCtrlFactory.cpp

#
# Abstract Base Class for all Experiment Controllers
#
    ExperimentCtrlView.cpp
    ExperimentCtrlModel.cpp

#
# Factory for creating the different experiment control models/views
#
    ExperimentCtrlFactory.hpp
    ExperimentCtrlFactory.cpp
)

#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE ExperimentCtrlView.hpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ExperimentCtrlModel.hpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET ExperimentControllers PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_include_directories(ExperimentControllers PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(ExperimentControllers PUBLIC ${EXPERIMENT_STATE_MACHINE_INCLUDE_DIR})

target_link_libraries(ExperimentControllers PRIVATE ${Qt}::Widgets)
target_link_libraries(ExperimentControllers PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(ExperimentControllers PRIVATE cbdf::ctrl)
target_link_libraries(ExperimentControllers PRIVATE SpidercamCtrl_static)
target_link_libraries(ExperimentControllers PRIVATE ExperimentStateMachine)

set_target_properties(ExperimentControllers PROPERTIES FOLDER "ExperimentControllers")


# Add the Spidercam Control library source code directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Spidercam)

