
# Add the public header files to use the library
set(SEPTENTRIO_PUBLIC_HEADERS
    SsnxFactory.hpp
)

# Add the source files used in the library
set(SEPTENTRIO_SOURCE_FILES
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
    SsnxStatusView.hpp
    SsnxStatusView.cpp

    SsnxFactory.hpp
    SsnxFactory.cpp
)


# The static library (.lib/.a) for incorporating GPS data in Ceres
add_library(septentrio_static STATIC)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(septentrio_static PROPERTIES OUTPUT_NAME "libSeptentrio")
else ()
	set_target_properties(septentrio_static PROPERTIES OUTPUT_NAME "Septentrio")
endif (MSVC)

set_target_properties(septentrio_static PROPERTIES LANGUAGE CXX)

target_compile_features(septentrio_static PRIVATE cxx_std_17)

target_sources(septentrio_static

PUBLIC
    ${SEPTENTRIO_PUBLIC_HEADERS}
 
PRIVATE
    ${SEPTENTRIO_SOURCE_FILES}
)

target_include_directories(septentrio_static PRIVATE ${COMMON_INCLUDE_DIR})
target_include_directories(septentrio_static PRIVATE ${UTILITIES_INCLUDE_DIR})

#target_link_libraries(septentrio_static PRIVATE cbdf::gps)

target_link_libraries(septentrio_static PUBLIC ${Qt}::Widgets)
target_link_libraries(septentrio_static PUBLIC ${Qt}::Network)
target_link_libraries(septentrio_static PUBLIC ${Qt}::SerialPort)
target_link_libraries(septentrio_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(septentrio_static PUBLIC ssnx2net::ssnx2net_static)
target_link_libraries(septentrio_static PUBLIC ssnx::ssnx_static)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET septentrio_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(septentrio_static PROPERTIES FOLDER "Sensors/GPS/Septertino")
