
# Define some shortcuts for include files.
set(BASE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(UTILITIES_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Utilities)
set(EXPERIMENT_CONTROLLERS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ExperimentControllers)
set(EXPERIMENT_STATE_MACHINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ExperimentStateMachine)
set(SENSORS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Sensors)

if(USE_INTERNAL_CBDF_LIB)
	set(BLOCK_DATA_FILE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/BlockDataFile)

	# Add the "block data file" directory
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/BlockDataFile)
endif()

# Add some basic utilities
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Utilities)

# Add the "experiment controller" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ExperimentControllers)

# Add the "experiment state machine" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ExperimentStateMachine)

# Add the "sensors" directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Sensors)

# Add the all of the applications directory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/apps)

