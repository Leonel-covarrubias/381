#include "raylib-cpp.hpp"
#include "rlgl.h"

struct Skybox{
    raylib::Texture texture; 
    raylib::Shader shader; 
    raylib::Model cube; 
    
    Skybox(): shader (0){} 


    void Init(){
    auto gen = raylib::Mesh::Cube(1,1,1); 
    cube =((raylib::Mesh*)(&gen))->LoadModelFrom(); 

    shader = raylib::Shader::LoadFromMemory (R"C++(
#version 330

// Input vertex attributes
in vec3 vertexPosition;

// Input uniform values
uniform mat4 matProjection;
uniform mat4 matView;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;

void main()
{
    // Calculate fragment position based on model transformations
    fragPosition = vertexPosition;

    // Remove translation from the view matrix
    mat4 rotView = mat4(mat3(matView));
    vec4 clipPos = matProjection*rotView*vec4(vertexPosition, 1.0);

    // Calculate final vertex position
    gl_Position = clipPos;
}

    )C++", R"C++(
#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Input uniform values
uniform samplerCube environmentMap;
uniform bool vflipped;
uniform bool doGamma;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Fetch color from texture map
    vec3 color = vec3(0.0);

    if (vflipped) color = texture(environmentMap, vec3(fragPosition.x, -fragPosition.y, fragPosition.z)).rgb;
    else color = texture(environmentMap, fragPosition).rgb;

    if (doGamma)// Apply gamma correction
    {
        color = color/(color + vec3(1.0));
        color = pow(color, vec3(1.0/2.2));
    }

    // Calculate final fragment color
    finalColor = vec4(color, 1.0);
}


    )C++"); 
    cube.materials[0].shader = shader; 
    shader.SetValue("environmentMap", (int) MATERIAL_MAP_CUBEMAP, SHADER_UNIFORM_INT ); 

    }
    void Load(std::string filename){
        shader.SetValue("doGamma", 0, SHADER_UNIFORM_INT); 
        shader.SetValue("vlipped", 0, SHADER_UNIFORM_INT); 


        raylib::Image img(filename); 
        texture.Load(img, CUBEMAP_LAYOUT_AUTO_DETECT); 
        texture.SetFilter(TEXTURE_FILTER_BILINEAR); 

        cube.materials[0].maps [ MATERIAL_MAP_CUBEMAP].texture = texture; 



    }

    void Draw(){ 
        rlDisableBackfaceCulling(); 
        rlDisableDepthMask(); 
        cube.Draw({});
        rlEnableBackfaceCulling();
        rlEnableDepthMask();  
    }

};


template <typename T>
concept Transfromer = requires(T t, raylib:: Transform m){
    {t.operator()(m) } -> std::convertible_to<raylib::Transform>; 
};

void DrawBoundedModel (raylib::Model& model, Transfromer auto transformer){
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
    raylib::Model ship = LoadModel ("meshes/SmitHouston_Tug.glb");
    raylib::Model imperial = LoadModel("meshes/sw-imperial-shuttle-lambda-docked.glb");
    raylib::Model defaultCube ("bad.obj"); 
    int x = 0; 
    while (!window.ShouldClose()){
        
        // int CenterX = window.GetWidth() / 2;
        // int CenterY= window.GetHeight() / 2;

        

        window.BeginDrawing();
        {
                window.ClearBackground(GRAY);
           
    
            camera.BeginMode();
            {
                // defaultCube.Draw({0,0,-10}); 

                DrawBoundedModel(plane, [](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate({0,0,0});
                });
                DrawBoundedModel(plane, [](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate({-100,100,0}).RotateY(raylib::Degree(180)).Scale(1,-1,1);
                    

                });


                DrawBoundedModel(ship, [](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate({200,0,0}).RotateY(raylib::Degree(90));
                });
                 DrawBoundedModel(ship, [](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate({-200,0,0});
                });
                 DrawBoundedModel(ship, [](raylib::Transform t) -> raylib ::Transform{
                    return t.Translate({100,100,0}).RotateY(raylib::Degree(270)).Scale(1,2,1);
                });

                
                DrawBoundedModel(imperial, [](raylib::Transform t) -> raylib ::Transform{
                  
                    return t.Translate({0,-150,0}).Scale(10,10,10).RotateZ(raylib::Degree(100));
                });

                
        
               // skybox.Draw(); 
            }
            camera.EndMode(); 
        }
        window.EndDrawing();
    }
}
