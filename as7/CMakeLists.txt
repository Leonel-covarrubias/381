cmake_minimum_required(VERSION 3.18)
project(as7 LANGUAGES CXX)

add_subdirectory(raylib-cpp)

file(GLOB sources "src/*.cpp" "src/*.c")
add_executable(as7 ${sources})
target_link_libraries(as7 PUBLIC raylib raylib_cpp raylib::buffered)
set_property(TARGET as7 PROPERTY CXX_STANDARD 20) 

configure_file(textures/skybox.png textures/skybox.png COPYONLY)
configure_file(textures/grass.jpg textures/grass.jpg COPYONLY)
configure_file(meshes/PolyPlane.glb meshes/PolyPlane.glb COPYONLY)
