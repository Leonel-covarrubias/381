#include "raylib-cpp.hpp"
#include "rlgl.h"
#include <memory> 
#include <iostream>
#include <cmath> 
#include <BufferedInput.hpp>
#include <cstdlib> 
#include <ctime>

#include <optional> 
#include "ECS.hpp"

//#include "skybox.hpp"

using namespace cs381;


template <typename T>
concept Transformer = requires(T t, raylib:: Transform m){
    {t.operator()(m) } -> std::convertible_to<raylib::Transform>; 
};

void DrawBoundedModel (raylib::Model& model, Transformer auto transformer, bool IsSelected){
    raylib::Transform backupTransform = model.transform; 
    model.transform = transformer(backupTransform); 
    model.Draw({});
    if(IsSelected){ 
        model.GetTransformedBoundingBox().Draw(); 
    }
    model.transform = backupTransform;
}

struct PhysicsProperties {
    float maxSpeed;
    float acceleration;
    float turningRate;

    PhysicsProperties(float maxSpeed, float acceleration, float turningRate)
        : maxSpeed(maxSpeed), acceleration(acceleration), turningRate(turningRate) {}
};



struct InputComponent {
    raylib::BufferedInput inputs; 
    bool IsSelected = false; 
    bool isAccelerating = false;
    bool isDecelerating = false;
    bool isTurningLeft = false;
    bool isTurningRight = false;
    bool isAccend = false; 
    bool isDecend = false; 
    bool stop = false; 
 
     InputComponent(Entity& e) {
        inputs["Accelerate"] = raylib::Action::key(KEY_W).SetPressedCallback([this]{
    if (IsSelected) {
        isAccelerating = true; 
    }
}).move(); 

inputs["AccelerateRelease"] = raylib::Action::key(KEY_W).SetReleasedCallback([this]{
    if (IsSelected) {
        isAccelerating = false; 
    }
}).move(); 

inputs["Decelerate"] = raylib::Action::key(KEY_S).SetPressedCallback([this]{
    if (IsSelected) {
        isDecelerating = true; 
    }
}).move(); 

inputs["DecelerateRelease"] = raylib::Action::key(KEY_S).SetReleasedCallback([this]{
    if (IsSelected) {
        isDecelerating = false; 
    }
}).move(); 

inputs["TurnLeft"] = raylib::Action::key(KEY_A).SetPressedCallback([this]{
    if (IsSelected) {
        isTurningLeft = true; 
    }
}).move(); 

inputs["TurnLeftRelease"] = raylib::Action::key(KEY_A).SetReleasedCallback([this]{
    if (IsSelected) {
        isTurningLeft = false; 
    }
}).move(); 

inputs["TurnRight"] = raylib::Action::key(KEY_D).SetPressedCallback([this]{
    if (IsSelected) {
        isTurningRight = true; 
    }
}).move(); 

inputs["TurnRightRelease"] = raylib::Action::key(KEY_D).SetReleasedCallback([this]{
    if (IsSelected) {
        isTurningRight = false; 
    }
}).move(); 

inputs["Stop"] = raylib::Action::key(KEY_SPACE).SetPressedCallback([this]{
    if (IsSelected) {
        stop = true; 
    }
}).move();

inputs["StopRelease"] = raylib::Action::key(KEY_SPACE).SetReleasedCallback([this]{
    if (IsSelected) {
        stop = false; 
    }
}).move();
inputs["Accend"] = raylib::Action::key(KEY_O).SetPressedCallback([this]{
    if (IsSelected) {
        isAccend = true; 
    }
}).move();

inputs["AccendRelease"] = raylib::Action::key(KEY_O).SetReleasedCallback([this]{
    if (IsSelected) {
        isAccend = false; 
    }
}).move();

inputs["Decend"] = raylib::Action::key(KEY_I).SetPressedCallback([this]{
    if (IsSelected) {
        isDecend = true; 
    }
}).move();

inputs["DecendRelease"] = raylib::Action::key(KEY_I).SetReleasedCallback([this]{
    if (IsSelected) {
        isDecend = false; 
    }
}).move();



    }
        

    void tick(float dt){

        inputs.PollEvents(); 
    }
};

struct Rendering{
    raylib::Model* model;
    bool drawBoundingBox = false; 
}; 

struct Physics{}; 


template <typename T>
void DrawSystem(Scene<T>& scene){
        for(Entity e = 0; e < scene.entityMasks.size(); e++){
            if (!scene.template HasComponent<Rendering>(e)) continue; 
            if( !scene.template HasComponent<Physics>(e)) continue;
            auto& rendering = scene.template GetComponent<Rendering>(e); 
            auto& physics = scene.template GetComponent<Physics>(e); 

            auto transformer = [](raylib::Transform t) -> raylib::Transform {
                return t; 
            }; 
            if(rendering.drawBoundingBox) 
                DrawBoundedModel(*rendering.model, transformer, true);
            else DrawBoundedModel(*rendering.model, transformer, false);
        }
}



int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    raylib::Window window (800, 800, "CS381 - Assignment 9");



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
    
    auto mesh = raylib::Mesh::Plane (10'000,10'000, 50, 50, 25); 
    raylib::Model ground = ((raylib::Mesh*)&mesh)->LoadModelFrom(); 
    raylib::Texture grass ("../textures/CustomWater.jpg"); 

    

    grass.SetFilter (TEXTURE_FILTER_BILINEAR); 
    grass.SetWrap(TEXTURE_WRAP_REPEAT); 
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grass; 

    raylib::Model plane("../meshes/PolyPlane.glb"); 

    InitAudioDevice(); 
    raylib::Music music("../audio/CustomMusic.mp3");

    Scene scene; 
    auto e = scene.CreateEntity(); 
    scene.AddComponent<Rendering>(e) = {&plane, true}; 

    int CenterX = window.GetWidth() / 2;
    int CenterY= window.GetHeight() / 2;

    bool Clicked = false; 


    Vector2 ballPosition = {0,0 };

    while (!window.ShouldClose()){


        music.SetVolume(5);
        music.Play(); 
        music.Update();


        ballPosition = GetMousePosition();
        
        window.BeginDrawing();
        {
            window.ClearBackground(GRAY);
             int textWidth = MeasureText("Bomb Chase (Press Enter to Start)", 30);
                int textHeight = 20;
                

                text.Draw("Click the screen - the Game ", 20, 29, 30, raylib::Color::DarkPurple());

                if (Clicked == true){
                    text.Draw("Woah you're good! Thanks for playing! ", ballPosition.x, ballPosition.y, 30, raylib::Color::DarkPurple());
                }
                else{
                    text.Draw("Click Here to play ", 20, 60, 30, raylib::Color::DarkPurple());
                }

         camera.BeginMode();
            {
                
             if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    Clicked = true ; 
    
                }

            ground.Draw({0,0,0}); 
            DrawSystem(scene); 

            }
            camera.EndMode();
           
                
        }
        window.EndDrawing();
        
    }
}

