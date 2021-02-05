cmake_minimum_required(VERSION 3.11)
set(PROJECT_NAME project_base)
project(${PROJECT_NAME})

function(watch)
    set_property(
            DIRECTORY
            APPEND
            PROPERTY CMAKE_CONFIGURE_DEPENDS ${ARGV}
    )
endfunction()

set(CMAKE_POLICY_DEFAULT_CMP0012 NEW)
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -O3")

file(GLOB SOURCES "src/*.cpp" "src/*.c" src/main.cpp)
file(GLOB HEADERS "include/*.h" "include/*.hpp")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(ASSIMP REQUIRED)

add_subdirectory(libs/glad)
add_subdirectory(libs/imgui)
add_subdirectory(libs/reactphysics3d)
add_subdirectory(tests)

add_definitions(${OPENGL_DEFINITIONS})

add_library(STB_IMAGE libs/stb_image.cpp)
set_source_files_properties(libs/stb_image.cpp include/stb_image.h
        PROPERTIES
        COMPILE_FLAGS
        "-Wno-shift-negative-value -Wno-implicit-fallthrough")

set(LIBS glfw glad OpenGL::GL X11 Xrandr Xinerama Xi Xxf86vm Xcursor dl pthread freetype ${ASSIMP_LIBRARIES} STB_IMAGE imgui reactphysics3d)


configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)


include_directories(include/)
add_executable(${PROJECT_NAME}
        ${SOURCES} include/ConcaveCollider.h src/Entity/Entity.cpp src/Entity/Entity.h src/Entity/Component.h src/Entity/EntityManager.cpp src/Entity/EntityManager.h src/Controller/PlayerController.cpp src/Controller/PlayerController.h src/Controller/RenderController.cpp src/Controller/RenderController.h src/Entity/ShaderManager.h src/constants.h include/common.cpp include/learnopengl/model.h.cpp  src/Controller/PhysicsController.h src/Controller/PhysicsController.cpp src/Controller/LevelController.cpp src/Controller/LevelController.h)

target_link_libraries(${PROJECT_NAME} ${LIBS})

# set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
file(GLOB SHADERS "shaders/*.vs"
        "shaders/*.fs")
foreach(SHADER ${SHADERS})
    # file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/shaders)
    watch(${SHADER})
endforeach()

