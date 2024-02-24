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

    float speed1 = 0; 
    float speed2 = 0; 
    float speed3 = 0; 
    float acceleration = 20; 
    raylib::Vector3 position1 = {10,0,0}; 
    raylib::Vector3 position2 = {10,0,0}; 
    raylib::Degree heading = 0; 
    raylib::Vector3 velocity1 = {0,0,0}; 
    raylib::Vector3 velocity2 = {0,0,0};

    int currentPlane = 1; 
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
                plane.Draw({50,10,10}); 
               
                DrawBoundedModel(plane, [&position1,&heading](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate(position1).RotateY(heading).Scale(2,2,2);
                });
                DrawBoundedModel(plane, [&position2,&heading](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate(position2).RotateY(heading).Scale(2,2,2);
                });

                if (IsKeyDown(KEY_TAB)) {
            // Switch between planes
           currentPlane = (currentPlane == 1) ? 2 : 1;
        }

        switch (currentPlane) {
            case 1:
                // Handle plane 1 controls
                if (IsKeyDown(KEY_A)) {
                    speed1 += acceleration * GetFrameTime();
                    velocity1.x = speed1;
                }
                if (IsKeyDown(KEY_D)) {
                    speed1 -= acceleration * GetFrameTime();
                    velocity1.x = speed1;
                }
                if (IsKeyDown(KEY_W)) {
                    speed2 += acceleration * GetFrameTime();
                    velocity1.z = speed2;
                }
                if (IsKeyDown(KEY_S)) {
                    speed2 -= acceleration * GetFrameTime();
                    velocity1.z = speed2;
                }
                if (IsKeyDown(KEY_Q)) {
                    speed2 += acceleration * GetFrameTime();
                    velocity1.y = speed2;
                }
                if (IsKeyDown(KEY_E)) {
                    speed2 -= acceleration * GetFrameTime();
                    velocity1.y = speed2;
                }
                break;
            case 2:
                // Handle plane 2 controls
                if (IsKeyDown(KEY_A)) {
                    speed1 += acceleration * GetFrameTime();
                    velocity2.x = speed1;
                }
                if (IsKeyDown(KEY_D)) {
                    speed1 -= acceleration * GetFrameTime();
                    velocity2.x = speed1;
                }
                if (IsKeyDown(KEY_W)) {
                    speed2 += acceleration * GetFrameTime();
                    velocity2.z = speed2;
                }
                if (IsKeyDown(KEY_S)) {
                    speed2 -= acceleration * GetFrameTime();
                    velocity2.z = speed2;
                }
                if (IsKeyDown(KEY_Q)) {
                    speed2 += acceleration * GetFrameTime();
                    velocity2.y = speed2;
                }
                if (IsKeyDown(KEY_E)) {
                    speed2 -= acceleration * GetFrameTime();
                    velocity2.y = speed2;
                }
                break;
        }

        // Reset speeds and velocities on space key press
        if (IsKeyPressed(KEY_SPACE)) {
            speed1 = 0.0f;
            speed2 = 0.0f;
            velocity1.x = 0; 
            velocity1.y = 0;
            velocity1.z = 0;
            // velocity2 = { 0,0,0};
        }

        // Update positions
        position1.x += velocity1.x * GetFrameTime();
        position1.y += velocity1.y * GetFrameTime();
        position1.z += velocity1.z * GetFrameTime();

        position2.x += velocity2.x * GetFrameTime();
        position2.y += velocity2.y * GetFrameTime();
        position2.z += velocity2.z * GetFrameTime();

            }
            camera.EndMode(); 
        }
        window.EndDrawing();
        
    }
}

