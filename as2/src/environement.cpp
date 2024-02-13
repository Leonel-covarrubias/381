#include "raylib-cpp.hpp"
#include "rlgl.h"
//#include "skybox.hpp"


template <typename T>
concept Transformer = requires(T t, raylib:: Transform m){
    {t.operator()(m) } -> std::convertible_to<raylib::Transform>; 
};

void DrawBoundedModel (raylib::Model& model, Transformer auto transformer){
    raylib::Transform backupTransform = model.transform; 
    model.transform = transformer(backupTransform); 
    model.Draw({});
    model.GetTransformedBoundingBox().Draw(); 
    model.transform = backupTransform;
}



int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    raylib::Window window (800, 800, "CS381 - Assignment 1");
    

    raylib::Model bad("bad.obj");

    raylib:: Camera camera(
        raylib::Vector3(0,120,-500),
        raylib::Vector3(0,0,0),
        raylib::Vector3::Up(),
        45,
        CAMERA_PERSPECTIVE
    );
   
    raylib::Text text;

    // Skybox skybox; 
    // skybox.Load("textures/skybox.png"); 
    raylib::Model plane = LoadModel ("meshes/PolyPlane.glb"); 
    auto mesh = raylib::Mesh::Plane (10'000,10'000, 50, 50, 25); 
    raylib::Model ground = ((raylib::Mesh*)&mesh)->LoadModelFrom(); 
    raylib::Texture grass ("../textures/grass.jpg"); 
    grass.SetFilter (TEXTURE_FILTER_BILINEAR); 
    grass.SetWrap(TEXTURE_WRAP_REPEAT); 
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grass; 

    float speed = 50; 
    raylib::Vector3 position = {0,0,0}; 
    raylib::Degree heading = 10; 
   // cs31::SkyBox skybox ("../textures/skybox.png"); 
   
    raylib::Model defaultCube ("bad.obj"); 
    float wtimer = 0; 
 
    while (!window.ShouldClose()){
        
      

        

        window.BeginDrawing();
        {
                window.ClearBackground(GRAY);
           
    
            camera.BeginMode();
            {
                window.ClearBackground(GRAY); 
                ground.Draw({0,0,0}); 
                DrawBoundedModel(plane, [&position,&heading](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate(position).RotateY(heading).Scale(10,10,10);
                });

                // if(IsKeyDown(KEY_W)){
                //     position += raylib::Vector3::Right()*window.GetFrameTime(); 
                   
                // } 

                raylib::Vector3 velocity = {speed* cos(heading.RadianValue()),0,-speed * sin(heading.RadianValue())}; 
                position += velocity * window.GetFrameTime(); 

                if(IsKeyPressed(KEY_A))
                    heading += 5; 
               
               //skybox.Draw(); 
            }
            camera.EndMode(); 
        }
        window.EndDrawing();
    }
}
