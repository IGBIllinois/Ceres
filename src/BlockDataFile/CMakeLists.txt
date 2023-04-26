
project(CeresBlockDataFile VERSION 1.0.0.0)


add_library(block_data_file_static STATIC)

set_target_properties(block_data_file_static PROPERTIES LANGUAGE CXX)

target_compile_features(block_data_file_static PRIVATE cxx_std_17)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
	set_target_properties(block_data_file_static PROPERTIES OUTPUT_NAME "libCeresBlockDataFile")
else ()
	set_target_properties(block_data_file_static PROPERTIES OUTPUT_NAME "CeresBlockDataFile")
endif (MSVC)


# Add the public header files to use the library
set(BLOCK_DATA_FILE_PUBLIC_HEADERS

#
# Base Headers for the parsing block data files
#

#	 BlockDataFileExceptions.hpp
#    BlockDataFile.hpp
#	 BlockId.hpp
#    Crc.hpp
#	 ClassIdentifiers.hpp
#    DataBuffer.hpp
#	 RawDataBuffer.hpp
#	 BlockParser.hpp

#
# Headers for parsing experiment data
#
#    ExperimentBlockId.hpp
#    ExperimentDataIdentifiers.hpp
#    ExperimentParser.hpp

#
# Headers for parsing Spidercam data
#
#    SpidercamBlockId.hpp
#    SpidercamDataIdentifiers.hpp
#    SpidercamParser.hpp

#
# Headers for parsing Ssnx GPS data
#
#    SsnxBlockId.hpp
#    SsnxDataIdentifiers.hpp
#    SsnxParser.hpp

#
# Headers for parsing HySpex data
#
    HySpexBlockId.hpp
    HySpexDataIdentifiers.hpp
    HySpexVNIR_3000N_Parser.hpp
    HySpexSWIR_384_Parser.hpp

#
# Headers for parsing OUSTER LiDAR data
#
#    OusterBlockId.hpp
#    OusterDataIdentifiers.hpp
#    OusterParser.hpp

#
# Headers for parsing Axis Communications
# webcam data
#
#	AxisCommunicationsBlockId.hpp
#	AxisDataIdentifiers.hpp
#	AxisCommunicationsParser.hpp
#	ImageBuffers.hpp

#
# Headers for parsing Weather data
#
#    WeatherBlockId.hpp
#    WeatherDataIdentifiers.hpp
#    WeatherParser.hpp

#
# Headers for parsing Position/Velocity/Time data
#
#    PvtBlockId.hpp
#    PvtDataIdentifiers.hpp
#    PvtParser.hpp
)


target_sources(block_data_file_static

PRIVATE

    ${BLOCK_DATA_FILE_PUBLIC_HEADERS}

#
# Base Headers for the serializing block data files
#
#	BlockSerializer.hpp

#
# Headers for serializing experiment data
#
#    ExperimentSerializer.hpp

#
# Headers for serializing Spidercam data
#
#    SpidercamSerializer.hpp

#
# Headers for serializing Ssnx GPS data
#
#    SsnxSerializer.hpp

#
# Headers for serializing HySpex data
#
    HySpexVNIR_3000N_Serializer.hpp
    HySpexSWIR_384_Serializer.hpp

#
# Headers for serializing OUSTER LiDAR data
#
#    OusterSerializer.hpp

#
# Headers for serializing Axis Communications
# webcam data
#
#	AxisCommunicationsSerializer.hpp

#
# Headers for serializing Weather data
#
#    WeatherSerializer.hpp

#
# Headers for serializing Position/Velocity/Time data
#
#    PvtSerializer.hpp

PRIVATE

#    PvtBlockId.cpp
#    PvtSerializer.cpp
#    PvtParser.cpp

#    WeatherBlockId.cpp
#    WeatherSerializer.cpp
#    WeatherParser.cpp

#	AxisCommunicationsBlockId.cpp
#	AxisCommunicationsSerializer.cpp
#	AxisCommunicationsParser.cpp
#	ImageBuffers.cpp

#    OusterBlockId.cpp
#    OusterSerializer.cpp
#    OusterParser.cpp

    HySpexBlockId.cpp
    HySpexVNIR_3000N_Serializer.cpp
    HySpexVNIR_3000N_Parser.cpp
    HySpexSWIR_384_Serializer.cpp
    HySpexSWIR_384_Parser.cpp

#    SsnxBlockId.cpp
#    SsnxSerializer.cpp
#    SsnxParser.cpp

#    SpidercamBlockId.cpp
#    SpidercamSerializer.cpp
#    SpidercamParser.cpp

#    ExperimentBlockId.cpp
#    ExperimentSerializer.cpp
#    ExperimentParser.cpp

#	BlockDataFile.cpp
#	Crc.cpp
#    DataBuffer.cpp
#    RawDataBuffer.cpp
#	BlockSerializer.cpp
#	BlockParser.cpp
)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET block_data_file_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_include_directories(block_data_file_static PRIVATE ${CMAKE_INSTALL_PREFIX}/include)

target_link_libraries(block_data_file_static PUBLIC ${Qt}::Multimedia)
target_link_libraries(block_data_file_static PRIVATE cbdf::cbdf)
#target_link_libraries(block_data_file_static PRIVATE cbdf::info)
target_link_libraries(block_data_file_static PRIVATE cbdf::sensors)
#target_link_libraries(block_data_file_static PRIVATE cbdf::weather)

set_target_properties(block_data_file_static PROPERTIES FOLDER "BlockDataFile")

#
# Install the Ceres Block Data File libraries
#

install(TARGETS block_data_file_static
	EXPORT ${PROJECT_NAME}-config
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT ${PROJECT_NAME}-config
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(FILES ${BLOCK_DATA_FILE_PUBLIC_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})


