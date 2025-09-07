#include <iostream>
#include <raylib.h>

using namespace std;

int main () {

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    int ball_x = 100;
    int ball_y = 100;
    int ball_speed_x = 5;
    int ball_speed_y = 5;
    int ball_radius = 15;

    cout << "Hello World" << endl;
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Office Gang");
    SetTargetFPS(60);
    int characterImgIndex = 0;
    int maxImgIndex = 6;
    Rectangle characterRecDes = {0, 0, 896/56*2, 640/20*2};
    while (WindowShouldClose() == false){
        Rectangle characterRecSrc = { characterImgIndex*896/56, 640/20*2, 896/56, 640/20};
   
        ball_x += ball_speed_x;
        ball_y += ball_speed_y;

        if(ball_x + ball_radius >= SCREEN_WIDTH || ball_x - ball_radius <= 0)
        {
            ball_speed_x *= -1;
        }

        if(ball_y + ball_radius >= SCREEN_HEIGHT || ball_y - ball_radius <= 0)
        {
            ball_speed_y *= -1;
        }
        Image mapImg = LoadImage("resources/image/office_gang_map.png");
        Texture2D texture = LoadTextureFromImage(mapImg);
        Image characterImg = LoadImage("resources/image/character/workingman2.png");
        Texture2D characterTexture = LoadTextureFromImage(characterImg);
        UnloadImage(mapImg);
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(texture, 0, 0, WHITE);
            DrawTexturePro(characterTexture, characterRecSrc, characterRecDes, {0,0}, 0, WHITE);
            // Free the image, texture now lives on GPU
            DrawCircle(ball_x,ball_y,ball_radius, WHITE);
            characterImgIndex++;
            if (characterImgIndex >= maxImgIndex) characterImgIndex = 0;

        EndDrawing();
    }

    CloseWindow();
}
