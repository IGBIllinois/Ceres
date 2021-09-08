
# Support inclusion of the nlohmann json library either as an external library
# or embedded the code here 

if (SPIDERCAM_USE_EXTERNAL_JSON)
	find_package(nlohmann_json 3.2 REQUIRED)
else()
	set(JSON_BuildTests OFF CACHE INTERNAL "")
	add_subdirectory(nlohmann_json)
endif()