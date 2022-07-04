
# Define some shortcuts for include files.
set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
set(BLOCK_DATA_FILE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/BlockDataFile)

# Add the "common" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/common)

# Add the "block data file" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/BlockDataFile)

# Add some basic utilities
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Utilities)

# Add the "experiment controller" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ExperimentControllers)

# Add the "sensors" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Sensors)

# Add the all of the applications directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/apps)

