cmake_minimum_required (VERSION 3.8)

set(MAIN_FILE main.cpp)
set(MAIN_EXEC stateVariablesCompareTool)
add_executable(${MAIN_EXEC} ${MAIN_FILE})

add_subdirectory(Common)
add_subdirectory(Core)
add_subdirectory(Filesystem)
add_subdirectory(Loader)
add_subdirectory(Third/CLI11)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(Third/nlohmann_json)

target_link_libraries(${MAIN_EXEC} Common)
target_link_libraries(${MAIN_EXEC} Loader Core)
target_link_libraries(${MAIN_EXEC} CLI11::CLI11)
if (NOT MSVC)
    target_link_libraries(${MAIN_EXEC} stdc++fs)
endif()
