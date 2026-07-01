#include <iostream>
#include <raylib.h>
#include "multiPlayer/client.h"
#include "game.h"
#include "globalVar.h"

int main () {
    bool isMultiPlayer = false;
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Office Gang");
    SetExitKey(KEY_NULL);
    InitAudioDevice();
    Music music = LoadMusicStream("resources/musics/5-Peaceful.ogg");
    PlayMusicStream(music);
    SetTargetFPS(60);
    HideCursor();
    Game game;
    if (!isMultiPlayer) {
        while (WindowShouldClose() == false){
            float deltaTime = GetFrameTime();
            game.tick(deltaTime, music);
        }
    } else {
        runMultiPlayer();
    }
    CloseWindow();
}
