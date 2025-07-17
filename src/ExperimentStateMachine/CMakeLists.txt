
set(CMAKE_AUTOMOC ON)


add_library(ExperimentStateMachine STATIC)

set_target_properties(ExperimentStateMachine PROPERTIES LANGUAGE CXX)

target_compile_features(ExperimentStateMachine PRIVATE cxx_std_20)

target_sources(ExperimentStateMachine
PUBLIC
    ExperimentTypes.hpp
    ExperimentState.hpp
    ExperimentStateCreator.hpp

PRIVATE

#
# Base States for the experiment state machine
#
    PauseExperimentStateDlg.hpp
    PauseExperimentStateDlg.cpp
    BasicExperimentStates.hpp
    BasicExperimentStates.cpp

    MarkerInterfaces.hpp
    MarkerExperimentStates.hpp
    MarkerExperimentStates.cpp

    ExperimentStateRemoteInterface.hpp
    ExperimentStateRemoteInterface.cpp

#
# A variable table for passing values to difference experiment states
# within the state machine
#
    ExperimentVariableTable.hpp
    ExperimentVariableTable.cpp

#
# The Experiment State Machine
#
	ExperimentStateMachine.hpp
	ExperimentStateMachine.cpp
)

#
# We skip these header files as they are public and MOC will be used 
# in the main application
#set_property(SOURCE ExperimentCtrlView.hpp PROPERTY SKIP_AUTOMOC ON)
#set_property(SOURCE ExperimentCtrlModel.hpp PROPERTY SKIP_AUTOMOC ON)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET ExperimentStateMachine PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_include_directories(ExperimentStateMachine PRIVATE ${COMMON_INCLUDE_DIR})

target_link_libraries(ExperimentStateMachine PRIVATE ${Qt}::Widgets)
target_link_libraries(ExperimentStateMachine PRIVATE nlohmann_json::nlohmann_json)

set_target_properties(ExperimentStateMachine PROPERTIES FOLDER "ExperimentStateMachine")


