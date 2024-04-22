# Install script for directory: /home/packnel87/GameDev/381/as9/raylib-cpp/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/AudioDevice.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/AudioStream.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/BoundingBox.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Camera2D.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Camera3D.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Color.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Degree.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Font.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Functions.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Gamepad.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Image.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Material.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Matrix.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Mesh.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Model.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/ModelAnimation.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Mouse.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Music.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Ray.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/RayCollision.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/RaylibException.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/raylib-cpp-utils.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/raylib-cpp.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/raylib.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/raymath.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Rectangle.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/RenderTexture.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Shader.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Sound.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Text.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Texture.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/TextureUnmanaged.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Touch.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Vector2.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Vector3.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Vector4.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/VrStereoConfig.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Wave.hpp"
    "/home/packnel87/GameDev/381/as9/raylib-cpp/include/Window.hpp"
    )
endif()

