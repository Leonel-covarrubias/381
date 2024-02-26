#include "raylib-cpp.hpp"
#include "rlgl.h"

#include <cstdlib> 
#include <ctime>
#include <iostream>


template <typename T>
concept Transformer = requires(T t, raylib:: Transform m){
    {t.operator()(m) } -> std::convertible_to<raylib::Transform>; 
};

void DrawBoundedModel (raylib::Model& model, Transformer auto transformer,int planeindex, int isPLane){
    raylib::Transform backupTransform = model.transform; 
    model.transform = transformer(backupTransform); 
    model.Draw({});
    if(planeindex == isPLane){ 
        model.GetTransformedBoundingBox().Draw(); 
    }
    model.transform = backupTransform;
}



int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    raylib::Window window (800, 800, "CS381 - Assignment 3");
    

    raylib::Model bad("bad.obj");

    raylib:: Camera camera(
        raylib::Vector3(0,120,-500),
        raylib::Vector3(0,0,0),
        raylib::Vector3::Up(),
        45,
        CAMERA_PERSPECTIVE
    );
   
    raylib::Text text;

    
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

    float flightspeed1, flightspeed2,flightspeed3= 0; 

    raylib::Vector3 position1 = {0,0,0}; 
    raylib::Vector3 position2 = {50,0,0}; 
    raylib::Vector3 position3 = {-50,0,0}; 

    raylib::Degree heading1 = 0; 
    raylib::Degree heading2 = 0; 
    raylib::Degree heading3 = 0; 

    srand(time(NULL));

    // Load cube model
    raylib::Model cube("bad.obj");

    // Initialize cube position and velocity
    raylib::Vector3 cubePosition = {0, 100, 0.0f};
    raylib::Vector3 cubeVelocity = {0.0f, -10.0f, 0.0f};

    
    int currentPlane = 1; 

    
   
   
    raylib::Model defaultCube ("bad.obj"); 
    float wtimer = 0; 
 
    while (!window.ShouldClose()){
        
        cubeVelocity.x = static_cast<float>(rand() % 3 - 1) * 5; // Random velocity in x direction
        cubeVelocity.y = static_cast<float>(rand() % 3 - 1) * 5; // Random velocity in y direction
        cubeVelocity.z = static_cast<float>(rand() % 3 - 1) * 5; // Random velocity in z direction

        // update bomb position 
        cubePosition.x += cubeVelocity.x;
        cubePosition.y += cubeVelocity.y;
        cubePosition.z += cubeVelocity.z;


        // The following clamps are for game boundaries 
        cubePosition.x = Clamp(cubePosition.x, -450.0f, 450.0f);
        cubePosition.y = Clamp(cubePosition.y, 0.0f, 200.0f);
        cubePosition.z = Clamp(cubePosition.z, -200.0f, 450.0f);

       
        position1.x = Clamp(position1.x, -450.0f, 450.0f);
        position1.y = Clamp(position1.y, 0.0f, 200.0f);
        position1.z = Clamp(position1.z, -200.0f, 450.0f);

        position2.x = Clamp(position2.x, -450.0f, 450.0f);
        position2.y = Clamp(position2.y, 0.0f, 200.0f);
        position2.z = Clamp(position2.z, -200.0f, 450.0f);

        position3.x = Clamp(position3.x, -450.0f, 450.0f);
        position3.y = Clamp(position3.y, 0.0f, 200.0f);
        position3.z = Clamp(position3.z, -200.0f, 450.0f);

        

        

        

        window.BeginDrawing();
        {
                window.ClearBackground(GRAY);
               

            camera.BeginMode();
            {
            
                window.ClearBackground(GRAY); 
                ground.Draw({0,0,0}); 

                //draw bomb model 
               
                DrawBoundedModel(cube, [&](raylib::Transform t) -> raylib::Transform {
                return t.Translate(cubePosition).Scale(15,15,15);
                }, 0, 0);
               
                // draw plane models 
                DrawBoundedModel(plane, [&position2,&heading2](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate(position2).RotateY(heading2).Scale(1,1,1);
                },2,currentPlane);
                 DrawBoundedModel(plane, [&position3,&heading3](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate(position3).RotateY(heading3).Scale(1,1,1);
                },3,currentPlane);
                DrawBoundedModel(plane, [&position1,&heading1](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate(position1).RotateY(heading1).Scale(1,1,1);
                },1,currentPlane);


                // Update plane positions 
                raylib::Vector3 velocity1 = {speed1 *cos(heading1.RadianValue()), flightspeed1, -speed1 *sin(heading1.RadianValue())}; 
                position1 += velocity1 * window.GetFrameTime();

                raylib::Vector3 velocity2 = {speed2 *cos(heading2.RadianValue()), flightspeed2, -speed2 *sin(heading2.RadianValue())}; 
                position2 += velocity2 * window.GetFrameTime();

                raylib::Vector3 velocity3 = {speed3 *cos(heading3.RadianValue()), flightspeed3, -speed3 *sin(heading3.RadianValue())}; 
                position3 += velocity3 * window.GetFrameTime();


                //collision detection, if its within range of a plane, then end program 
                
                 if ((std::abs(cubePosition.x - position1.x) <= 30.0f &&
             std::abs(cubePosition.y - position1.y) <= 10.0f &&
             std::abs(cubePosition.z - position1.z) <= 30.0f) ||
            (std::abs(cubePosition.x - position2.x) <= 30.0f &&
             std::abs(cubePosition.y - position2.y) <= 10.0f &&
             std::abs(cubePosition.z - position2.z) <= 30.0f) ||
            (std::abs(cubePosition.x - position3.x) <= 30.0f &&
             std::abs(cubePosition.y - position3.y) <= 10.0f &&
             std::abs(cubePosition.z - position3.z) <= 30.0f)) {
            
            exit(EXIT_SUCCESS); 
        }


                // Plane controls implimented below 

        switch (currentPlane) {
            case 1:
                
                if (IsKeyDown(KEY_A)) {
                    heading1 += 5 ; 
    
                }
                if (IsKeyDown(KEY_D)) {
                     heading1 -= 5 ; 
    
                }
                if (IsKeyDown(KEY_W)) {
                    speed1 += acceleration * GetFrameTime();
                }
                if (IsKeyDown(KEY_S)) {
                    speed1 -= acceleration * GetFrameTime();
                    
                }
                if (IsKeyDown(KEY_Q)) {
                    flightspeed1 += acceleration * GetFrameTime();
                   
                }
                if (IsKeyDown(KEY_E)) {
                    flightspeed1 -= acceleration * GetFrameTime();
                }
                if (IsKeyPressed(KEY_SPACE)) {
                    velocity1.x = 0; 
                    velocity1.y = 0;
                    velocity1.z = 0;
                    speed1 = 0;
                    flightspeed1 = 0; } 
                    
                 if (IsKeyPressed(KEY_TAB)) {
                    velocity1.x = 0; 
                    velocity1.y = 0;
                    velocity1.z = 0;
                    speed1 = 0;
                    flightspeed1 = 0; } 
                        
                break;
            case 2:
                
                if (IsKeyDown(KEY_A)) {
                    heading2 += 5 ; 
    
                }
                if (IsKeyDown(KEY_D)) {
                     heading2 -= 5 ; 
    
                }
                if (IsKeyDown(KEY_W)) {
                    speed2 += acceleration * GetFrameTime();
                }
                if (IsKeyDown(KEY_S)) {
                    speed2 -= acceleration * GetFrameTime();
                    
                }
                if (IsKeyDown(KEY_Q)) {
                    flightspeed2 += acceleration * GetFrameTime();
                   
                }
                if (IsKeyDown(KEY_E)) {
                     flightspeed2 -= acceleration * GetFrameTime();
                }
                if (IsKeyPressed(KEY_SPACE)) {
                    velocity2.x = 0; 
                    velocity2.y = 0;
                    velocity2.z = 0;
                    speed2 = 0;
                    flightspeed2 = 0;  } 
                if (IsKeyPressed(KEY_TAB)) {
                    velocity2.x = 0; 
                    velocity2.y = 0;
                    velocity2.z = 0;
                    speed2 = 0;
                    flightspeed2 = 0;  }     
                break;
            case 3:
               if (IsKeyDown(KEY_A)) {
                    heading3 += 5 ; 
    
                }
                if (IsKeyDown(KEY_D)) {
                     heading3 -= 5 ; 
    
                }
                if (IsKeyDown(KEY_W)) {
                    speed3 += acceleration * GetFrameTime();
                }
                if (IsKeyDown(KEY_S)) {
                    speed3 -= acceleration * GetFrameTime();
                    
                }
               if (IsKeyDown(KEY_Q)) {
                    flightspeed3 += acceleration * GetFrameTime();
                   
                }
                if (IsKeyDown(KEY_E)) {
                     flightspeed3 -= acceleration * GetFrameTime();
                }
                if (IsKeyPressed(KEY_SPACE)) {
                    velocity3.x = 0; 
                    velocity3.y = 0;
                    velocity3.z = 0;
                    speed3 = 0;
                    flightspeed3 = 0; } 
                if (IsKeyPressed(KEY_TAB)) {
                    velocity3.x = 0; 
                    velocity3.y = 0;
                    velocity3.z = 0;
                    speed3 = 0;
                    flightspeed3 = 0; }    
                break;
        }


        // If tab is pressed, select the next plane  
      if (IsKeyPressed (KEY_TAB)) {
            
                 if(currentPlane == 3){
                    currentPlane = 1;
                 }
                 else{
                    currentPlane++; 
                 }
                 
                }


            }
            camera.EndMode(); 
        }
        window.EndDrawing();
        
    }
}

