
set(CMAKE_AUTOMOC ON)


add_library(ExperimentControllers STATIC)

#set_target_properties(ExperimentControllers PROPERTIES LANGUAGE C CXX)

target_compile_features(ExperimentControllers PRIVATE cxx_std_17)

target_sources(ExperimentControllers
PUBLIC

#
# Base Headers for all Experiment Controller classes
#
    ExperimentCtrlView.hpp
    ExperimentCtrlModel.hpp
 
PRIVATE
#
# Spidercam files
#
    SpidercamView.hpp
    SpidercamView.cpp
    SpidercamModel.hpp
    SpidercamModel.cpp
    SpidercamCtrl.hpp
    SpidercamCtrl.cpp
    SpidercamUtils.hpp
    SpidercamUtils.cpp

    SpidercamCtrlFactory.hpp
    SpidercamCtrlFactory.cpp

#
# Abstract Base Class for all Experiment Controllers
#
    ExperimentCtrlIdentifiers.hpp

    ExperimentCtrlView.cpp
    ExperimentCtrlModel.cpp
)

#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE ExperimentCtrlView.hpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ExperimentCtrlModel.hpp PROPERTY SKIP_AUTOMOC ON)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET ExperimentControllers PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_link_libraries(ExperimentControllers PRIVATE ${Qt}::Widgets)
target_link_libraries(ExperimentControllers PUBLIC ${Qt}::Network)
target_link_libraries(ExperimentControllers PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(ExperimentControllers PRIVATE spidercam::scConnect_static)
target_link_libraries(ExperimentControllers PRIVATE BlockDataFile)

