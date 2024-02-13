cmake_minimum_required(VERSION 3.18)
project(as1 LANGUAGES CXX)

add_subdirectory(raylib-cpp)

file(GLOB sources "src/*.cpp" "src/*.c")
add_executable(as1 ${sources})
target_link_libraries(as1 PUBLIC raylib raylib_cpp)
set_property(TARGET as1 PROPERTY CXX_STANDARD 20) 

configure_file(textures/skybox.png textures/skybox.png COPYONLY)
configure_file(meshes/PolyPlane.glb meshes/PolyPlane.glb COPYONLY)
configure_file(meshes/SmitHouston_Tug.glb meshes/SmitHouston_Tug.glb COPYONLY)
configure_file(meshes/sw-imperial-shuttle-lambda-docked.glb meshes/sw-imperial-shuttle-lambda-docked.glb COPYONLY)