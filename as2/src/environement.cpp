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
    raylib::Window window (800, 800, "CS381 - Assignment 2");
    

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
    raylib::Texture grass ("textures/grass.jpg"); 

    

    grass.SetFilter (TEXTURE_FILTER_BILINEAR); 
    grass.SetWrap(TEXTURE_WRAP_REPEAT); 
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grass; 

    float speed = 0; 
    float speed2 = 0; 
    float speed3 = 0; 
    float acceleration = 20; 
    raylib::Vector3 position = {10,0,0}; 
    raylib::Degree heading = 0; 
    raylib::Vector3 velocity = {0,0,0}; 
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
                    return t.Translate(position).RotateY(heading).Scale(2,2,2);
                });

                // if(IsKeyDown(KEY_W)){
                //     position += raylib::Vector3::Right()*window.GetFrameTime(); 
                   
                // } 
               
             
               

                if(IsKeyDown(KEY_A)){
                    speed += acceleration * window.GetFrameTime(); 
                    velocity.x = speed; 
                    
                }

                
                if(IsKeyDown(KEY_D)){
                    speed -= acceleration * window.GetFrameTime(); 
                    velocity.x = speed ; 
                }
                if(IsKeyDown(KEY_W)){
                     speed2 += acceleration * window.GetFrameTime(); 

                    velocity.z = speed2;
                }
                if(IsKeyDown(KEY_S)){
                    speed2 -= acceleration * window.GetFrameTime(); 
                   
                    velocity.z = speed2;
                }
                if(IsKeyDown(KEY_Q)){
                     speed3 += acceleration * window.GetFrameTime(); 
                   
                    velocity.y = speed3;
                }
                 if(IsKeyDown(KEY_E)){
                    speed3 -= acceleration * window.GetFrameTime(); 
                   
                    velocity.y = speed3;
                }
                    
                if(IsKeyPressed (KEY_SPACE)){
                    velocity.x = 0; 
                    velocity.y = 0;
                    velocity.z = 0;
                    
                    speed = 0;
                    speed2 = 0 ;
                    speed3 = 0; 
                    
                    
                }   

                position += velocity * window.GetFrameTime(); 
               //skybox.Draw(); 
            }
            camera.EndMode(); 
        }
        window.EndDrawing();
        
    }
}

