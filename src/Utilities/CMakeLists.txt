

add_library(Utilities STATIC)

#set_target_properties(Utilities PROPERTIES LANGUAGE C CXX)

target_compile_features(Utilities PRIVATE cxx_std_17)

target_sources(Utilities
PUBLIC

	Constants.hpp
	ColorGradient.hpp

PRIVATE

	ColorGradient.cpp
)

#
# Due to Qt's license, we must use the Qt DLLs.  For Windows, we must use MSVC dynamic runtime libraries
# Force use MultiThreadedDLL (-MD) and MultiThreadedDebugDLL (-MDd)
set_property(TARGET Utilities PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
