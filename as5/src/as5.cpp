#include "raylib-cpp.hpp"


#include <iostream>



void PingButton(){ 
    std::cout << "ping" <<std::endl; 
}
#define GUI_VOLUMECONTROL_IMPLEMENTATION 
#include "VolumeControl.h"


int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    raylib::Window window (300, 350, "CS381 - Assignment 5");
    InitAudioDevice(); 
    raylib::Sound ping("../audio/ping.wav"); 
    raylib::Music music("../audio/price-of-freedom.mp3");
    raylib::Music dialog("../audio/crowd.wav"); 
    auto guiState = InitGuiVolumeControl(); 
    
    

    while(!window.ShouldClose()){
        window.BeginDrawing();
        {
        window.ClearBackground(RAYWHITE);
        
            dialog.SetVolume(guiState.DialogueSliderValue/50); 
            dialog.Play(); 
            dialog.Update(); 
            music.SetVolume(guiState.MusicSliderValue/50);
            music.Play(); 
            music.Update(); 


            GuiVolumeControl(&guiState); 
            if (IsKeyPressed(KEY_SPACE)) {
                    PingButton();
                    ping.SetVolume(guiState.SFXSliderValue / 50); 
                    ping.Play(); 
                     
            }  
            
        }
        window.EndDrawing();
        } 
}
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"