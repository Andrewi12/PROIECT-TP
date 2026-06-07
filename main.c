#include "raylib.h"
#include <stdio.h> // Pentru a putea formata textul scorului

#define MAP_WIDTH 28
#define MAP_HEIGHT 36
#define TILE_SIZE 20 

typedef enum GameScreen { MENU, GAMEPLAY } GameScreen;

// 1 = Perete, 2 = Bulină, 0 = Spațiu gol
int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
    {0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,1,1,1,0,0,1,1,1,0,1,1,2,1,0,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,0,1,0,0,0,0,0,0,1,0,1,1,2,1,1,1,1,1,1},
    {0,0,0,0,0,0,2,0,0,0,1,0,0,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,0,1,0,0,0,0,0,0,1,0,1,1,2,1,1,1,1,1,1},
    {0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
    {0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,1,1,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,1,1,2,2,2,1},
    {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
    {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
    {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int main(void) {
    int screenWidth = MAP_WIDTH * TILE_SIZE;
    int screenHeight = MAP_HEIGHT * TILE_SIZE;
    
    InitWindow(screenWidth, screenHeight, "Proiect Pac-Man");
    SetTargetFPS(60); // Am redus viteza jocului la cerințele clasice

    GameScreen currentScreen = MENU;

    int pacmanX = 13; 
    int pacmanY = 19; 
    
    int score = 0; // Variabila pentru scor
    
    // Contor pentru a încetini mișcarea lui Pac-Man pe grilă (altfel zboară instant)
    int frameCounter = 0;
    int moveDelay = 8; 

    Rectangle playButton = { screenWidth/2.0f - 100, screenHeight/2.0f, 200, 60 };

    while (!WindowShouldClose()) {
        
        switch(currentScreen) {
            case MENU:
                if (CheckCollisionPointRec(GetMousePosition(), playButton)) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        currentScreen = GAMEPLAY;
                    }
                }
                break;

            case GAMEPLAY:
                frameCounter++;
                
                // Pac-Man se mișcă doar la fiecare 8 cadre (moveDelay) pentru a putea fi controlat
                if (frameCounter >= moveDelay) {
                    int nextX = pacmanX;
                    int nextY = pacmanY;

                    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) nextX++;
                    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  nextX--;
                    else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    nextY--;
                    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  nextY++;

                    // Limităm ieșirea de pe ecran
                    if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
                        
                        // Verificăm coliziunea cu peretele
                        if (map[nextY][nextX] != 1) {
                            pacmanX = nextX;
                            pacmanY = nextY;
                            frameCounter = 0; // Resetăm contorul de mișcare
                            
                            // Dacă pe noua poziție este o bulină (2)
                            if (map[pacmanY][pacmanX] == 2) {
                                score += 10;                // Creștem scorul
                                map[pacmanY][pacmanX] = 0;  // Ștergem bulina de pe hartă
                            }
                        }
                    }
                }
                break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch(currentScreen) {
            case MENU:
                DrawText("PAC-MAN", screenWidth/2 - MeasureText("PAC-MAN", 60)/2, screenHeight/4, 60, YELLOW);
                
                Color buttonColor = DARKGRAY;
                if (CheckCollisionPointRec(GetMousePosition(), playButton)) {
                    buttonColor = GRAY; 
                }
                
                DrawRectangleRec(playButton, buttonColor);
                DrawText("PLAY", playButton.x + 55, playButton.y + 15, 30, WHITE);
                break;

            case GAMEPLAY:
                for (int y = 0; y < MAP_HEIGHT; y++) {
                    for (int x = 0; x < MAP_WIDTH; x++) {
                        
                        // Desenăm pereții
                        if (map[y][x] == 1) {
                            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBLUE);
                        }
                        // Desenăm bulinele
                        else if (map[y][x] == 2) {
                            DrawCircle(x * TILE_SIZE + TILE_SIZE/2, y * TILE_SIZE + TILE_SIZE/2, 3, WHITE);
                        }
                    }
                }

                // Desenăm scorul sus
                DrawText(TextFormat("SCORE: %d", score), 10, 10, 20, WHITE);

                // Desenăm personajul
                int drawX = pacmanX * TILE_SIZE + TILE_SIZE / 2;
                int drawY = pacmanY * TILE_SIZE + TILE_SIZE / 2;
                DrawCircle(drawX, drawY, TILE_SIZE / 2 - 2, YELLOW);
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}