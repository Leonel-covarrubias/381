#include "raylib-cpp.hpp"

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    raylib::Window window (800, 800, "CS381 - Assignment 0");
   
   
    raylib::Text text;

    while (!window.ShouldClose()){
        
        int CenterX = window.GetWidth() / 2;
        int CenterY= window.GetHeight() / 2;

        window.BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);
           
            int textWidth = MeasureText("Please give me a good Grade", 27);
            int textHeight = 20;
            int textX = CenterX - (textWidth / 2);
            int textY = CenterY- (textHeight / 2);
            
            text.Draw("Please give me a good Grade", textX, textY, 27, raylib::Color::DarkPurple());
        }
        window.EndDrawing();
    }
}