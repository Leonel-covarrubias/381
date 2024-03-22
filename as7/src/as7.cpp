#include "raylib-cpp.hpp"
#include "rlgl.h"
#include <memory> 
#include <iostream>
#include <cmath> 
#include <BufferedInput.hpp>
#include <cstdlib> 
#include <ctime>

#include <optional> 
//#include "skybox.hpp"

class Entity;

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

struct Component {
    struct Entity* object; 

    Component(struct Entity& e) : object(&e){}

    virtual void setup() = 0; 
    virtual void cleanup() = 0; 
    virtual void tick(float dt) = 0; 
}; 
struct PhysicsProperties {
    float maxSpeed;
    float acceleration;
    float turningRate;

    PhysicsProperties(float maxSpeed, float acceleration, float turningRate)
        : maxSpeed(maxSpeed), acceleration(acceleration), turningRate(turningRate) {}
};


struct TransformComponent : public Component{
    using Component::Component; 
    raylib ::Vector3 position = {0,0,0}; 
    raylib::Degree heading = 0; 
    raylib::Vector3 scale = (1,1,1); 

    void setup() override{}
    void cleanup()override{}
    void tick(float dt) override{
        position.x = Clamp(position.x, -450.0f, 450.0f);
        position.y = Clamp(position.y, 0.0f, 200.0f);
        position.z = Clamp(position.z, -200.0f, 450.0f);

    }

};

struct Entity{
    std::vector<std::unique_ptr<Component>> components; 

    Entity() {AddComponent<TransformComponent>(); }
    Entity (const Entity&) = delete; 
    Entity(Entity&& other) : components(std::move(other.components)){
        for (auto& c:components)
            c->object = this; 
    }

    template <std::derived_from<Component> T, typename... Ts> 
    size_t AddComponent(Ts... args){
        auto c = std::make_unique<T>(*this, std::forward<Ts> (args)...); 
        components.push_back(std::move(c)); 
        return components.size()-1; 
    }

    template<std::derived_from<Component> T>
    std::optional<std::reference_wrapper<T>> getComponent(){
        if constexpr(std::is_same_v<T, TransformComponent>){  
            T* cast = dynamic_cast <T*> (components[0].get()); 
            if (cast) return *cast; 
        }
        for (auto& c: components){
            T* cast = dynamic_cast <T*> (c.get()); 
            if (cast) return *cast; 
        }

        return std::nullopt; 
    }
    void tick(float dt){
        for(auto& c:components)
            c->tick(dt); 
    }
}; 

struct RenderingComponent : public Component{

    raylib::Model model; 
    bool drawBox= false; 

    RenderingComponent(Entity& e, raylib::Model&& model): Component(e), model(std::move(model)) { }

    void setup() override{}
    void cleanup()override{}

    void tick(float dt) override{
        auto ref = object->getComponent<TransformComponent>(); 
        if(!ref) return; 
        auto& transform = ref->get(); 

       raylib::Vector3 position = transform.position; 
       raylib::Degree heading = transform.heading; 
       raylib::Vector3 scale = transform.scale; 

        DrawBoundedModel(model, [&position,&heading, &scale](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate(position).RotateY(heading).Scale(scale.x,scale.y,scale.z);
                },drawBox);
    }

};

struct InputComponent : public Component {
    raylib::BufferedInput inputs; 
    bool IsSelected = false; 
    bool isAccelerating = false;
    bool isDecelerating = false;
    bool isTurningLeft = false;
    bool isTurningRight = false;
    bool isAccend = false; 
    bool isDecend = false; 
    bool stop = false; 
 
     InputComponent(Entity& e) : Component(e){
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
        

    void setup() override {}
    void cleanup() override {}

    void tick(float dt) override {

        inputs.PollEvents(); 
    }
};

struct PhysicsComponent : public Component {
    PhysicsProperties properties;
    raylib::Vector3 velocity = {0, 0, 0};
    float rotation = 0; 
    float speed, flightspeed = 0; 
    

    PhysicsComponent(Entity& e, const PhysicsProperties& props) : Component(e), properties(props) {}

    void setup() override {}
    void cleanup() override {}
    void tick(float dt) override {
        
        
        auto input = object->getComponent<InputComponent>();
        

        
        orientedUpdate(dt, speed, flightspeed, properties.maxSpeed, properties.acceleration, properties.turningRate,
                       input->get().isAccelerating, input->get().isDecelerating, input->get().isTurningLeft, input->get().isTurningRight,input->get().isAccend, input->get().isDecend ,input->get().stop);
    }

   
    void orientedUpdate(float dt, float& speed, float& flightspeed, float maxSpeed, float acceleration, float turningRate,
                    bool isAccelerating, bool isDecelerating, bool isTurningLeft, bool isTurningRight, bool isAccend, bool isDecend, bool stop){
    
    auto transform = object->getComponent<TransformComponent>();

    raylib::Degree heading = transform->get().heading; 

   
    if (isAccelerating) {
        speed +=  acceleration * dt;
        speed = std::clamp(speed, -maxSpeed, maxSpeed);
    }

    
    if (isDecelerating) {
        speed -= acceleration * dt; 
        speed = std::clamp(speed, -maxSpeed, maxSpeed);
    }

    if(isAccend){
        flightspeed += 2* acceleration * dt; 
    }
    if (isDecend){
        flightspeed -= 2* acceleration *dt; 
    }


    
    if (isTurningLeft) {
       heading += turningRate ; 
    }
    if (isTurningRight) {
        heading -= turningRate; } 

    if (heading >= 360) {
    heading -= 360;
    } else if (heading < 0) {
    heading += 360;
    }
    if (stop) {
        speed = 0; 
        } 

    raylib::Vector3 velocity = {speed *cos(heading.RadianValue()), flightspeed, -speed *sin(heading.RadianValue())}; 
    transform->get().heading = heading; 
    transform->get().position += velocity * dt;
}
}; 

struct RandomPhysicsComponent : public Component {
    
    raylib::Vector3 Velocity = {0.0f, 0.0f, 0.0f};
    

    RandomPhysicsComponent(Entity& e) : Component(e) {}



    void setup() override {}
    void cleanup() override {}
    void tick(float dt) override {
    auto transform = object->getComponent<TransformComponent>();
    Velocity.x = static_cast<float>(rand() % 3 - 1) * 100; // Random velocity in x direction
    Velocity.y = static_cast<float>(rand() % 3 - 1) * -100; // Random velocity in y direction
    Velocity.z = static_cast<float>(rand() % 3 - 1) * 100; // Random velocity in z direction
        
    transform->get().position += Velocity * dt;
    
    }
}; 

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    raylib::Window window (800, 800, "CS381 - Assignment 6");



    std::vector<Entity> entities; 
    std::vector<Entity> npcEntities; 

    Entity& e = entities.emplace_back(); 
    raylib::Model plane = LoadModel ("../meshes/PolyPlane.glb"); 
    e.AddComponent<RenderingComponent>(std::move(plane));
    e.getComponent<RenderingComponent>()->get().drawBox = true ;
    e.getComponent<TransformComponent>()->get().position = raylib::Vector3(100, 0, 0);
    e.getComponent<TransformComponent>()->get().scale = raylib::Vector3(1, 1, 1);
    PhysicsProperties planeproperties = {100,15,15}; // (max speed, acceleration, turningspeed) 
    e.AddComponent<InputComponent>();
    e.getComponent<InputComponent>()->get().IsSelected = true;
    e.AddComponent<PhysicsComponent>(planeproperties);
    

    Entity& plane2 = entities.emplace_back(); 
    raylib::Model plane1 = LoadModel ("meshes/PolyPlane.glb"); 
    plane2.AddComponent<RenderingComponent>(std::move(plane1));
    plane2.getComponent<TransformComponent>()->get().position = raylib::Vector3(0, 0, 0);
    plane2.AddComponent<InputComponent>();
    plane2.AddComponent<PhysicsComponent>(planeproperties);
    plane2.getComponent<TransformComponent>()->get().scale = raylib::Vector3(1, 1, 1);

    Entity& plane3 = entities.emplace_back(); 
    raylib::Model plane2mod = LoadModel ("meshes/PolyPlane.glb"); 
    plane3.AddComponent<RenderingComponent>(std::move(plane2mod));
    plane3.getComponent<TransformComponent>()->get().position = raylib::Vector3(-100, 0, 0);
    plane3.getComponent<TransformComponent>()->get().scale = raylib::Vector3(1, 1, 1);
    plane3.AddComponent<InputComponent>();
    plane3.AddComponent<PhysicsComponent>(planeproperties);

    Entity& bomb = npcEntities.emplace_back(); 
    raylib::Model bombmod = LoadModel ("../meshes/bomb.glb"); 
    bomb.AddComponent<RenderingComponent>(std::move(bombmod));
    bomb.getComponent<TransformComponent>()->get().position = raylib::Vector3(0, 100, 0);
    bomb.getComponent<TransformComponent>()->get().scale = raylib::Vector3(15, 15, 15);
    bomb.AddComponent<RandomPhysicsComponent>(); 


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
    raylib::Texture grass ("../textures/water.jpg"); 

    

    grass.SetFilter (TEXTURE_FILTER_BILINEAR); 
    grass.SetWrap(TEXTURE_WRAP_REPEAT); 
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grass; 

    
    int currentEntityIndex = 0; 
    bool startGame, nextEntity, gameOver = false; 

    raylib::BufferedInput inputs; 
    inputs["Start"] = raylib::Action::key(KEY_ENTER).SetPressedCallback([&startGame]{
        startGame = true; 
    }).move(); 
    inputs["Next"] = raylib::Action::key(KEY_TAB).SetPressedCallback([&nextEntity]{
        nextEntity = true; 
    }).move();
    inputs["NextRest"] = raylib::Action::key(KEY_TAB).SetReleasedCallback([&nextEntity]{
        nextEntity = false; 
    }).move();

    
    float wtimer = 0; 
    InitAudioDevice(); 
    raylib::Music music("../audio/price-of-freedom.mp3");

    while (!window.ShouldClose()){


        inputs.PollEvents(); 
        music.SetVolume(5);
        music.Play(); 
        music.Update();
        int CenterX = window.GetWidth() / 2;
        int CenterY= window.GetHeight() / 2;
        window.BeginDrawing();
        {
                window.ClearBackground(GRAY);
            if(startGame == false && gameOver == false ){
                int textWidth = MeasureText("Bomb Chase (Press Enter to Start)", 30);
                int textHeight = 20;
                int textX = CenterX - (textWidth / 2);
                int textY = CenterY- (textHeight / 2);
            
                text.Draw("Bomb Chase (Press Enter to Start)", textX, textY, 30, raylib::Color::DarkPurple()); 
            } 
            if(startGame == true && gameOver == true ){
                int textWidth = MeasureText("GAME OVER :(", 10);
                int textHeight = 20;
                int textX = CenterX - (textWidth / 2);
                int textY = CenterY- (textHeight / 2);
            
                text.Draw("GAME OVER :(", textX, textY, 30, raylib::Color::DarkPurple()); 
            } 

            
        
            if(startGame == true && gameOver == false ){
                camera.BeginMode();
            {
               
                 if (nextEntity) {
                    entities[currentEntityIndex].getComponent<InputComponent>()->get().IsSelected = false;
                    entities[currentEntityIndex].getComponent<RenderingComponent>()->get().drawBox = false; 
                    entities[currentEntityIndex].getComponent<InputComponent>()->get().stop = true; 
                    currentEntityIndex = (currentEntityIndex + 1) % entities.size();
                    entities[currentEntityIndex].getComponent<InputComponent>()->get().stop = false;
                    entities[currentEntityIndex].getComponent<InputComponent>()->get().IsSelected = true;
                    entities[currentEntityIndex].getComponent<RenderingComponent>()->get().drawBox = true ; 
                }
                ground.Draw({0,0,0}); 
               
               
               for (auto& e : npcEntities) {
                e.tick(window.GetFrameTime());
                
                for (auto& entity : entities) {
                entity.tick(window.GetFrameTime());
                if (std::abs(e.getComponent<TransformComponent>()->get().position.x - entity.getComponent<TransformComponent>()->get().position.x) <= 30.0f &&
                    std::abs(e.getComponent<TransformComponent>()->get().position.y - entity.getComponent<TransformComponent>()->get().position.y) <= 10.0f &&
                    std::abs(e.getComponent<TransformComponent>()->get().position.z - entity.getComponent<TransformComponent>()->get().position.z) <= 30.0f){
                        gameOver = true; 
                    }
               }
            }

            }
            camera.EndMode(); 
            }}
        window.EndDrawing();
        
    }
}

