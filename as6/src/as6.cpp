#include "raylib-cpp.hpp"
#include "rlgl.h"
#include <memory> 
#include <iostream>
#include <cmath> 
#include <BufferedInput.hpp>

#include <optional> 
//#include "skybox.hpp"

class Entity;

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
     raylib::Quaternion rotation = raylib::Quaternion::Identity();
     raylib::Vector3 rot = {0,0,0}; 

    void setup() override{}
    void cleanup()override{}
    void tick(float dt) override{}

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
        transform.position; 
        auto [axis, angle] = transform.rotation.ToAxisAngle(); 

        model.Draw(transform.position, axis, angle);
        
        if(drawBox ==true){
            model.GetTransformedBoundingBox().Draw(); 
        }
    }
};

struct InputComponent : public Component {
    raylib::BufferedInput inputs; 
    bool IsSelected = false; 
    bool isAccelerating = false;
    bool isDecelerating = false;
    bool isTurningLeft = false;
    bool isTurningRight = false;
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
    float speed = 0; 

    PhysicsComponent(Entity& e, const PhysicsProperties& props) : Component(e), properties(props) {}

    void setup() override {}
    void cleanup() override {}
    void tick(float dt) override {
        
        
        auto input = object->getComponent<InputComponent>();
        

        
        orientedUpdate(dt, speed, properties.maxSpeed, properties.acceleration, properties.turningRate,
                       input->get().isAccelerating, input->get().isDecelerating, input->get().isTurningLeft, input->get().isTurningRight,input->get().stop);
    }

   
    void orientedUpdate(float dt, float& speed, float maxSpeed, float acceleration, float turningRate,
                    bool isAccelerating, bool isDecelerating, bool isTurningLeft, bool isTurningRight, bool stop){
    
    auto transform = object->getComponent<TransformComponent>();

    auto [axis, heading1] = transform->get().rotation.ToAxisAngle();
    float radian = heading1;
    raylib::Degree heading = radian * 180.0 / M_PI;



   
    if (isAccelerating) {
        speed +=  acceleration * dt;
        speed = std::clamp(speed, -maxSpeed, maxSpeed);
    }

    
    if (isDecelerating) {
        speed -= acceleration * dt; 
        speed = std::clamp(speed, -maxSpeed, maxSpeed);
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

    raylib::Vector3 velocity = {speed *cos(heading.RadianValue()), 0, -speed *sin(heading.RadianValue())}; 
    transform->get().rotation = raylib::Quaternion::FromAxisAngle(raylib::Vector3{0, 1, 0}, heading.RadianValue());
    transform->get().position += velocity * dt;
}
}; 

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    raylib::Window window (800, 800, "CS381 - Assignment 6");



    std::vector<Entity> entities; 
    Entity& e = entities.emplace_back(); 
    raylib::Model plane = LoadModel ("meshes/PolyPlane.glb"); 
    e.AddComponent<RenderingComponent>(std::move(plane));
    e.getComponent<RenderingComponent>()->get().drawBox = true ;
    e.getComponent<TransformComponent>()->get().position = raylib::Vector3(100, 50, 0);
    PhysicsProperties planeproperties = {100,15,15}; // (max speed, acceleration, turningspeed) 
    e.AddComponent<InputComponent>();
    e.getComponent<InputComponent>()->get().IsSelected = true;
    e.AddComponent<PhysicsComponent>(planeproperties);

    Entity& plane2 = entities.emplace_back(); 
    raylib::Model plane1 = LoadModel ("meshes/PolyPlane.glb"); 
    plane2.AddComponent<RenderingComponent>(std::move(plane1));
    plane2.getComponent<TransformComponent>()->get().position = raylib::Vector3(0, 50, 0);
    plane2.AddComponent<InputComponent>();
    plane2.AddComponent<PhysicsComponent>(planeproperties);

    Entity& plane3 = entities.emplace_back(); 
    raylib::Model plane2mod = LoadModel ("meshes/PolyPlane.glb"); 
    plane3.AddComponent<RenderingComponent>(std::move(plane2mod));
    plane3.getComponent<TransformComponent>()->get().position = raylib::Vector3(50, 50, 0);
    plane3.AddComponent<InputComponent>();
    plane3.AddComponent<PhysicsComponent>(planeproperties);

    Entity& plane4 = entities.emplace_back(); 
    raylib::Model plane3mod = LoadModel ("meshes/PolyPlane.glb"); 
    plane4.AddComponent<RenderingComponent>(std::move(plane3mod));
    plane4.getComponent<TransformComponent>()->get().position = raylib::Vector3(-50, 50, 0);
    plane4.AddComponent<InputComponent>();
    plane4.AddComponent<PhysicsComponent>(planeproperties); 

    Entity& plane5 = entities.emplace_back(); 
    raylib::Model plane4mod = LoadModel ("meshes/PolyPlane.glb"); 
    plane5.AddComponent<RenderingComponent>(std::move(plane4mod));
    plane5.getComponent<TransformComponent>()->get().position = raylib::Vector3(-100, 50, 0);
    plane5.AddComponent<InputComponent>();
    plane5.AddComponent<PhysicsComponent>(planeproperties);


    Entity& ship1 = entities.emplace_back(); 
    raylib::Model ship1model = LoadModel ("../meshes/SmitHouston_Tug.glb"); 
    ship1.AddComponent<RenderingComponent>(std::move(ship1model));
    ship1.getComponent<TransformComponent>()->get().position = raylib::Vector3(0, 0, 0);
    ship1.AddComponent<InputComponent>();
    PhysicsProperties ship1prop = {100,20,20};
    ship1.AddComponent<PhysicsComponent>(ship1prop);

    Entity& ship2 = entities.emplace_back(); 
    raylib::Model ship2Model = LoadModel ("../meshes/ddg51.glb"); 
    ship2.AddComponent<RenderingComponent>(std::move(ship2Model));
    ship2.getComponent<TransformComponent>()->get().position = raylib::Vector3(50, 0, 0); 
    ship2.AddComponent<InputComponent>();
    PhysicsProperties ship2prop = {50,15,15};
    ship2.AddComponent<PhysicsComponent>(ship2prop);
    
    Entity& ship3 = entities.emplace_back(); 
    raylib::Model ship3Model = LoadModel ("../meshes/Container_ShipLarge.glb");
    ship3.AddComponent<RenderingComponent>(std::move(ship3Model));
    ship3.getComponent<TransformComponent>()->get().position = raylib::Vector3(100, 0, 0);
    ship3.AddComponent<InputComponent>();
    PhysicsProperties ship3prop = {20, 20, 5};
    ship3.AddComponent<PhysicsComponent>(ship3prop);

    Entity& ship4 = entities.emplace_back(); 
    raylib::Model ship4Model = LoadModel ("../meshes/SmitHouston_Tug.glb");
    ship4.AddComponent<RenderingComponent>(std::move(ship4Model));
    ship4.getComponent<TransformComponent>()->get().position = raylib::Vector3(-50, 0, 0);
    ship4.AddComponent<InputComponent>();
    PhysicsProperties ship4prop = {5, 5, 1};
    ship4.AddComponent<PhysicsComponent>(ship4prop);

    Entity& ship5 = entities.emplace_back(); 
    raylib::Model ship5Model = LoadModel ("../meshes/OilTanker.glb");
    ship5.AddComponent<RenderingComponent>(std::move(ship5Model));
    ship5.getComponent<TransformComponent>()->get().position = raylib::Vector3(-100, 0, 0);
    ship5.AddComponent<InputComponent>();
    PhysicsProperties ship5prop = {30, 30, 10};
    ship5.AddComponent<PhysicsComponent>(ship5prop);





    


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

    
    float wtimer = 0; 
    InitAudioDevice(); 
    raylib::Music music("../audio/price-of-freedom.mp3");

    while (!window.ShouldClose()){


            music.SetVolume(5);
            music.Play(); 
            music.Update();

        window.BeginDrawing();
        {
                window.ClearBackground(GRAY);
               

            camera.BeginMode();
            {
                 if (IsKeyPressed(KEY_TAB)) {
                    entities[currentEntityIndex].getComponent<InputComponent>()->get().IsSelected = false;
                    entities[currentEntityIndex].getComponent<RenderingComponent>()->get().drawBox = false; 
                    currentEntityIndex = (currentEntityIndex + 1) % entities.size();
                    entities[currentEntityIndex].getComponent<InputComponent>()->get().IsSelected = true;
                    entities[currentEntityIndex].getComponent<RenderingComponent>()->get().drawBox = true ; 
                }
                ground.Draw({0,0,0}); 
               
               for (auto& entity : entities) {
                entity.tick(window.GetFrameTime());
            }

            }
            camera.EndMode(); 
        }
        window.EndDrawing();
        
    }
}

