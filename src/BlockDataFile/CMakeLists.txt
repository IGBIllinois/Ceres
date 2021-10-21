

add_library(BlockDataFile STATIC)

#set_target_properties(BlockDataFile PROPERTIES LANGUAGE C CXX)

target_compile_features(BlockDataFile PRIVATE cxx_std_17)

target_sources(BlockDataFile
PUBLIC

	BlockDataFile.hpp
	BlockIdentifiers.hpp
    DataBuffer.hpp
	RawDataBuffer.hpp

PRIVATE

	BlockDataFile.cpp
    DataBuffer.cpp
    RawDataBuffer.cpp
)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET BlockDataFile PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
