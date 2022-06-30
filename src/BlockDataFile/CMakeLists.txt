

add_library(BlockDataFile STATIC)

set_target_properties(BlockDataFile PROPERTIES LANGUAGE CXX)

target_compile_features(BlockDataFile PRIVATE cxx_std_17)

target_sources(BlockDataFile
#PUBLIC
#INTERFACE
PRIVATE

#
# Base Headers for the serializing/parsing block data files
#

	BlockDataFile.hpp
	BlockId.hpp
	ClassIdentifiers.hpp
    DataBuffer.hpp
	RawDataBuffer.hpp
	BlockSerializer.hpp
	BlockParser.hpp

#
# Headers for serializing/parsing experiment data
#
    ExperimentBlockId.hpp
    ExperimentDataIdentifiers.hpp
    ExperimentSerializer.hpp

#
# Headers for serializing/parsing Spidercam data
#
    SpidercamBlockId.hpp
    SpidercamDataIdentifiers.hpp
    SpidercamSerializer.hpp

#
# Headers for serializing/parsing Ssnx GPS data
#
    SsnxBlockId.hpp
    SsnxDataIdentifiers.hpp
    SsnxSerializer.hpp

#
# Headers for serializing/parsing HySpex data
#
    HySpexBlockId.hpp
    HySpexDataIdentifiers.hpp
    HySpexVNIR_3000N_Serializer.hpp
    HySpexSWIR_384_Serializer.hpp

#
# Headers for serializing/parsing OUSTER LiDAR data
#
    OusterBlockId.hpp
    OusterDataIdentifiers.hpp
    OusterSerializer.hpp
    OusterParser.hpp

#
# Headers for serializing/parsing Axis Communications
# webcam data
#
	AxisCommunicationsBlockId.hpp
	AxisDataIdentifiers.hpp
	AxisCommunicationsSerializer.hpp

#
# Headers for serializing/parsing Weather data
#
    WeatherBlockId.hpp
    WeatherDataIdentifiers.hpp
    WeatherSerializer.hpp

#
# Headers for serializing/parsing Position/Velocity/Time data
#
    PvtBlockId.hpp
    PvtDataIdentifiers.hpp
    PvtSerializer.hpp
    PvtParser.hpp

PRIVATE

    PvtBlockId.cpp
    PvtSerializer.cpp
    PvtParser.cpp

    WeatherBlockId.cpp
    WeatherSerializer.cpp

	AxisCommunicationsBlockId.cpp
	AxisCommunicationsSerializer.cpp

    OusterBlockId.cpp
    OusterSerializer.cpp
    OusterParser.cpp

    HySpexBlockId.cpp
    HySpexVNIR_3000N_Serializer.cpp
    HySpexSWIR_384_Serializer.cpp

    SsnxBlockId.cpp
    SsnxSerializer.cpp

    SpidercamBlockId.cpp
    SpidercamSerializer.cpp

    ExperimentBlockId.cpp
    ExperimentSerializer.cpp

	BlockDataFile.cpp
    DataBuffer.cpp
    RawDataBuffer.cpp
	BlockSerializer.cpp
	BlockParser.cpp
)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET BlockDataFile PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_include_directories(BlockDataFile PRIVATE ${CMAKE_INSTALL_PREFIX}/include)

target_link_libraries(BlockDataFile PUBLIC ${Qt}::Multimedia)

