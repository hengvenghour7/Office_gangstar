// Prevent Windows API conflicts
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Avoid collisions
#define NOGDI           // This prevents GDI functions like Rectangle()
#define NOUSER          // This prevents user functions like CloseWindow, ShowCursor

// Include Raylib first
#include "raylib.h"
#include "raymath.h"

// Now include Boost
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "client.h"

using boost::asio::ip::tcp;

void runMultiPlayer() {
    // --- Initialize Raylib window ---
    InitWindow(800, 600, "Boost + Raylib Test");
    SetTargetFPS(60);
    Vector2 location {20,20};
    Vector2 player2Location {0,0};

    try {
        boost::asio::io_context io;

        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve("127.0.0.1", "197");
        tcp::socket socket(io);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connection established!\n";
        std::string line;
        std::string receievedText{" "};
        
        while (!WindowShouldClose()) {
            // --- Draw simple background ---
            int key = GetCharPressed();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Type a message in console...", 50, 50, 20, DARKGRAY);
            DrawText(line.c_str(), 50, 100, 20, DARKGRAY);
            DrawRectangle(location.x,location.y, 20, 20, GREEN);
            DrawRectangle(player2Location.x,player2Location.y, 20, 20, RED);
            if (IsKeyDown(KEY_D)) {
                location.x+=1;
                std::string moveMsg = "Move " + std::to_string(location.x) + " " + std::to_string(location.y);
                std::cout << "Server echo: " << player2Location.x << "\n";
                boost::asio::write(socket, boost::asio::buffer(moveMsg));
            }
            if ((key >= 32) && (key <= 125)) { // printable characters
                line += static_cast<char>(key);
            }
            // if (IsKeyDown(KEY_BACKSPACE) && !line.empty()) {
            //     line.pop_back();
            // }
            // if (IsKeyPressed(KEY_ENTER)) {
            //         // line = "this the new A line";
            //         boost::asio::write(socket, boost::asio::buffer(line));
            //     }
            if (socket.available()) {
                char reply[1024];
                int x,y;
                size_t len = socket.read_some(boost::asio::buffer(reply));
                sscanf(std::string(reply, len).c_str() , "Move %d %d", &x, &y );
                player2Location.x = (float)x;
                player2Location.y = (float)y;
                receievedText = std::string(reply, len);
            }
            DrawText(receievedText.c_str(), 50, 100, 20, DARKGRAY);
            EndDrawing();

            // --- Handle console input without freezing the window ---
        }

        CloseWindow();

    } catch (std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << "\n";
    } 
}
