
# Add the public header files to use the library
set(GPS_PUBLIC_HEADERS
    SsnxFactory.hpp
)

# Add the source files used in the library
set(GPS_SOURCE_FILES
#
# Different Septertino GPS Models based on data source
#

#	Network stream from relay program on dolly
    SsnxGpsStream.hpp
    SsnxGpsStream.cpp
    SsnxModel_net.hpp
    SsnxModel_net.cpp

#	Directly connected to GPS receiver's com port
    SsnxModel_direct.hpp
    SsnxModel_direct.cpp

#
# Septertino GPS receiver files
#
    SsnxModel.hpp
    SsnxModel.cpp
    SsnxView.hpp
    SsnxView.cpp

    SsnxFactory.cpp

#
# General GPS View-Model-Controller files
#
	GpsTypes.hpp
    GpsView.hpp
    GpsView.cpp
    GpsModel.hpp
    GpsModel.cpp
)


# The static library (.lib/.a) for incorporating GPS data in Ceres
add_library(gps_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(gps_static PROPERTIES OUTPUT_NAME "libGPS")
else ()
	set_target_properties(gps_static PROPERTIES OUTPUT_NAME "GPS")
endif (MSVC)

set_target_properties(gps_static PROPERTIES LANGUAGE CXX)

target_compile_features(gps_static PRIVATE cxx_std_17)

target_sources(gps_static

PUBLIC
    ${GPS_PUBLIC_HEADERS}
 
PRIVATE
    ${GPS_SOURCE_FILES}
)

target_include_directories(gps_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(gps_static PRIVATE ${BLOCK_DATA_FILE_INCLUDE_DIR})

target_link_libraries(gps_static PUBLIC ${Qt}::Widgets)
target_link_libraries(gps_static PUBLIC ${Qt}::Network)
target_link_libraries(gps_static PUBLIC ${Qt}::SerialPort)
target_link_libraries(gps_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(gps_static PUBLIC BlockDataFile)
target_link_libraries(gps_static PUBLIC ssnx2net::ssnx2net_static)
target_link_libraries(gps_static PUBLIC ssnx::ssnx_static)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET gps_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(gps_static PROPERTIES FOLDER "Sensors/GPS")
