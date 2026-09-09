
# Add the public header files to use the library
set(WEATHER_PUBLIC_HEADERS
    WeatherDataFactory.hpp
)

# Add the public header files to use the library
set(WEATHER_SOURCE_FILES

    WeatherDataIDs.hpp

#
# Weather Station at the Energy Farm
#
    WindSpeedDirection_StatusBar.hpp
    WindSpeedDirection_StatusBar.cpp

    WeatherDataModel_Http_Wind.hpp
    WeatherDataModel_Http_Wind.cpp

    WindTempRhPAR_StatusBar.hpp
    WindTempRhPAR_StatusBar.cpp

    WeatherDataModel_Http_Wind_T_RH_PAR.hpp
    WeatherDataModel_Http_Wind_T_RH_PAR.cpp

#
# Weather Station
#
    WeatherDataModel_Http.hpp
    WeatherDataModel_Http.cpp
    WeatherDataModel.hpp
    WeatherDataModel.cpp

    WeatherDataFactory.cpp
)


#
# We skip these header files as they are public and MOC will be used 
# in the main application
#set_property(SOURCE ExperimentCtrlView.hpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ExperimentState.hpp PROPERTY SKIP_AUTOMOC ON)


# The static library (.lib/.a) for communicating with Weather Stations
add_library(weather_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(weather_static PROPERTIES OUTPUT_NAME "libWeather")
else ()
	set_target_properties(weather_static PROPERTIES OUTPUT_NAME "Weather")
endif (MSVC)

set_target_properties(weather_static PROPERTIES LANGUAGE CXX)

target_compile_features(weather_static PRIVATE cxx_std_20)

target_sources(weather_static

PUBLIC
    ${WEATHER_PUBLIC_HEADERS}
 
PRIVATE
    ${WEATHER_SOURCE_FILES}
)

target_include_directories(weather_static PRIVATE ${COMMON_INCLUDE_DIR})

target_link_libraries(weather_static PRIVATE cbdf::weather)

target_link_libraries(weather_static PUBLIC ${Qt}::Widgets)
target_link_libraries(weather_static PUBLIC ${Qt}::Network)
target_link_libraries(weather_static PRIVATE nlohmann_json::nlohmann_json)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET weather_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(weather_static PROPERTIES FOLDER "Sensors/Weather")
