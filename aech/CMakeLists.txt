set(CMAKE_CXX_STANDARD 20)

file(GLOB PROJECT_HEADERS *.hpp *.h)
file(GLOB PROJECT_SOURCES *.cpp)
file(GLOB PROJECT_SHADERS shader/*.comp
        shaders/*.frag
        shaders/*.geom
        shaders/*.vert)
file(GLOB_RECURSE PROJECT_RESOURCES resources/*)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set_property(TARGET aech PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")
target_include_directories(aech PRIVATE ${STB_INCLUDE_DIRS})
target_link_libraries(aech PRIVATE glfw)
target_link_libraries(aech PRIVATE glad::glad)
target_link_libraries(aech PRIVATE assimp::assimp)
