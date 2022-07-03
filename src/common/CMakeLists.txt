set(CMAKE_AUTOMOC ON)


add_library(common STATIC)

#set_target_properties(Sensors PROPERTIES LANGUAGE C CXX)

target_compile_features(common PRIVATE cxx_std_17)

target_sources(common

PRIVATE
#
# General Application files
#
    SensorFactory.hpp

    DataModel.hpp
    DataModel.cpp
	
    DataThread.hpp
    DataThread.cpp
	
    TimestampProvider.hpp
    TimestampProvider.cpp
	
    CeresSplashScreen.hpp
    CeresSplashScreen.cpp
)

#
# We skip these header files as they are public and MOC will be used 
# in the main application
set_property(SOURCE CeresSplashScreen.hpp PROPERTY SKIP_AUTOMOC ON)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET common PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_link_libraries(common PRIVATE ${Qt}::Widgets)
target_link_libraries(common PRIVATE nlohmann_json::nlohmann_json)


#set_target_properties(Sensors PROPERTIES FOLDER "Sensors")



