# This CMake file is intended to register project-wide objects.
# This allows for reuse between deployments, or other projects.

add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Components")

if(FPRIME_ARDUINO)
    add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/MissionDeployment/")
else()
    add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/LocalDeployment/")
endif()
