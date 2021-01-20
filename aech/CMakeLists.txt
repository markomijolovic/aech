set(CMAKE_CXX_STANDARD 20)

file(GLOB PROJECT_HEADERS *.hpp *.h)
file(GLOB PROJECT_SOURCES *.cpp)
file(GLOB PROJECT_SHADERS 
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
set_source_files_properties(${PROJECT_RESOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set_property(TARGET aech PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")
target_include_directories(aech PRIVATE ${STB_INCLUDE_DIRS})
target_link_libraries(aech PRIVATE glfw)
target_link_libraries(aech PRIVATE glad::glad)
target_link_libraries(aech PRIVATE assimp::assimp)

set_target_properties(${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
		DEPENDS ${PROJECT_SHADERS})

add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
		DEPENDS ${PROJECT_RESOURCES})
