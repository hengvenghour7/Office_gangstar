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
    class Character
    {
    protected:
        float width;
        float height;
        float x;
        float y;
        float desX;
        float desY;
        float speed;
        int maxCols;
        int colIndex;
        int rowIndex;
        Rectangle characterRecDes;
        Rectangle characterRecSrc;
        Image characterImg;
        Texture2D characterTexture;
    public:
        Character (const char * imageTexture) {
            width = 896/56;
            height = 640/20;
            x = 0;
            y = 0;
            desX = 0;
            desY = 0;
            maxCols = 6;
            colIndex = 0;
            rowIndex = 2;
            speed = 2;
            characterImg = LoadImage(imageTexture);
            characterTexture = LoadTextureFromImage(characterImg);
            characterRecDes = {desX, desY, width*2, height*2};
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
        }
        void updateAnimation () {
            colIndex++;
            if (colIndex > maxCols - 1) colIndex = 0;
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
        }
        void updateCharacterMovement () {
            if (IsKeyDown(KEY_A)) {
                desX -= speed;
            }
            if (IsKeyDown(KEY_D)) {
                desX += speed;
            }
            if (IsKeyDown(KEY_W)) desY -= speed;
            if (IsKeyDown(KEY_S)) desY += speed;
            characterRecDes = {desX, desY, width*2, height*2};
        }
        void drawImage () {
            DrawTexturePro(characterTexture, characterRecSrc, characterRecDes, {0,0}, 0, WHITE);
        }
        void tryLog () {
            cout << "creating enw world";
        }
        void updateCharacterProgess () {
            drawImage();
            updateAnimation();
            updateCharacterMovement();
        }
    };
    class Player: public Character
    {
    private:
        /* data */
    public:
        Player (const char * imageTexture): Character(imageTexture) {
            
        }
        void updateAnimation () {
            colIndex++;
            if (colIndex > maxCols - 1) colIndex = 0;
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
        }
    };
    
    class AIPlayer : public Character {
        private:
            
        
        public: 
            AIPlayer (const char * imageTexture) : Character(imageTexture) {
            
        }
        void updateAnimation () {
            colIndex++;
            if (colIndex > maxCols - 1) colIndex = 0;
            characterRecSrc = { colIndex*width, rowIndex*height, width, height};
        }
        void updateCharacterProgess () {
            drawImage();
            updateAnimation();
        }
    };
    Player character1("resources/image/character/workingman2.png");
    AIPlayer character2("resources/image/character/workingman.png");
    int characterImgIndex = 0;
    int maxImgIndex = 6;
    while (WindowShouldClose() == false){   
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
        UnloadImage(mapImg);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE);
        character1.updateCharacterProgess();
        character2.updateCharacterProgess();
            // Free the image, texture now lives on GPU
            DrawCircle(ball_x,ball_y,ball_radius, WHITE);
            characterImgIndex++;
            if (characterImgIndex >= maxImgIndex) characterImgIndex = 0;

        EndDrawing();
    }

    CloseWindow();
}
