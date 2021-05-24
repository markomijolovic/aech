set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE PROJECT_HEADERS *.hpp *.h)
file(GLOB_RECURSE PROJECT_SOURCES *.cpp)
file(GLOB_RECURSE PROJECT_SHADERS
        shaders/*.glsl
        shaders/*.comp
        shaders/*.frag
        shaders/*.geom
        shaders/*.vert)
file(GLOB_RECURSE PROJECT_RESOURCES resources/*)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Dependencies" FILES ${DEPENDENCY_SOURCES})
source_group("Resources" FILES ${PROJECT_RESOURCES})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

include_directories(/ src/components src/ecs src/events src/glad src/graphics src/imgui src/samples src/managers src/math src/meshes src/misc src/rendering src/resource_management src/stb src/systems)
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")
target_include_directories(aech PRIVATE ${STB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
        DEPENDS ${PROJECT_SHADERS})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
        DEPENDS ${PROJECT_RESOURCES})
