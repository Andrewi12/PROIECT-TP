#include "raylib.h"
#include <stdio.h>
#include <stdlib.h> 

#define MAP_WIDTH 28
#define MAP_HEIGHT 36
#define TILE_SIZE 20 

typedef enum GameScreen { MENU, GAMEPLAY, ENDSCREEN, GAMEOVER } GameScreen;
typedef enum GhostMode { SCATTER, CHASE } GhostMode;

typedef struct Ghost {
    int id; 
    int x;
    int y;
    int dirX; 
    int dirY; 
    Color color;
    int scatterX; 
    int scatterY;
} Ghost;

// 1 = Perete, 2 = Bulină, 3 = Cireașă (Power Pellet), 0 = Spațiu gol
int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,3,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,3,1}, // Cirese sus (colturi)
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
    {1,3,2,2,1,1,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,1,1,2,2,3,1}, // Cirese jos (colturi)
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

int GetDistanceSq(int x1, int y1, int x2, int y2) {
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

int main(void) {
    int screenWidth = MAP_WIDTH * TILE_SIZE;
    int screenHeight = MAP_HEIGHT * TILE_SIZE;
    
    InitWindow(screenWidth, screenHeight, "Proiect Pac-Man");
    SetTargetFPS(60); 

    GameScreen currentScreen = MENU;

    int pacmanX = 13; 
    int pacmanY = 26; 
    int pacDirX = 1;
    int pacDirY = 0;
    
    Ghost ghosts[4];
    ghosts[0] = (Ghost){0, 13, 11, 1, 0, RED, MAP_WIDTH - 2, 1};                     
    ghosts[1] = (Ghost){1, 11, 14, 1, 0, SKYBLUE, MAP_WIDTH - 2, MAP_HEIGHT - 2};    
    ghosts[2] = (Ghost){2, 13, 14, 0, -1, MAGENTA, 1, 1};                            
    ghosts[3] = (Ghost){3, 15, 14, -1, 0, ORANGE, 1, MAP_HEIGHT - 2};                

    GhostMode globalMode = SCATTER;
    int modeTimer = 0; 
    
    int frightenedTimer = 0; // NOU: Timer-ul pentru cireșe
    
    int score = 0; 
    int totalDots = 0;
    
    // Numărăm toate bulinele (inclusiv cireșele)
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == 2 || map[y][x] == 3) totalDots++;
        }
    }
    
    int frameCounter = 0;
    int ghostFrameCounter = 0;
    int moveDelay = 8; 
    int ghostMoveDelay = 10; 

    Rectangle playButton = { screenWidth/2.0f - 100, screenHeight/2.0f, 200, 60 };

    while (!WindowShouldClose()) {
        
        switch(currentScreen) {
            case MENU:
                if (CheckCollisionPointRec(GetMousePosition(), playButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = GAMEPLAY;
                }
                break;

            case GAMEPLAY:
                // Scădem din timer-ul de vulnerabilitate în fiecare cadru
                if (frightenedTimer > 0) {
                    frightenedTimer--;
                }

                modeTimer++;
                if (globalMode == SCATTER && modeTimer > 420) {
                    globalMode = CHASE;
                    modeTimer = 0;
                } else if (globalMode == CHASE && modeTimer > 1200) {
                    globalMode = SCATTER;
                    modeTimer = 0;
                }

                // --- 1. UPDATE PAC-MAN ---
                frameCounter++;
                if (frameCounter >= moveDelay) {
                    int nextX = pacmanX;
                    int nextY = pacmanY;

                    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { nextX++; pacDirX = 1; pacDirY = 0; }
                    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  { nextX--; pacDirX = -1; pacDirY = 0; }
                    else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    { nextY--; pacDirX = 0; pacDirY = -1; }
                    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  { nextY++; pacDirX = 0; pacDirY = 1; }

                    if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
                        if (map[nextY][nextX] != 1) {
                            pacmanX = nextX; pacmanY = nextY;
                            frameCounter = 0; 
                            
                            // Mănâncă Bulină
                            if (map[pacmanY][pacmanX] == 2) {
                                score += 10;                
                                map[pacmanY][pacmanX] = 0;  
                                totalDots--; 
                            }
                            // Mănâncă Cireașă (Power Pellet)
                            else if (map[pacmanY][pacmanX] == 3) {
                                score += 50;
                                map[pacmanY][pacmanX] = 0;
                                totalDots--;
                                frightenedTimer = 600; // Fantomele devin vulnerabile timp de 10 secunde
                            }
                            
                            if (totalDots == 0) currentScreen = ENDSCREEN; 
                        }
                    }
                }

                // --- 2. UPDATE FANTOME ---
                ghostFrameCounter++;
                if (ghostFrameCounter >= ghostMoveDelay) {
                    
                    for (int g = 0; g < 4; g++) {
                        Ghost* gh = &ghosts[g];
                        
                        int targetX = gh->scatterX;
                        int targetY = gh->scatterY;

                        // Dacă fantomele sunt vulnerabile, forțăm destinația pe colțurile de Scatter
                        if (frightenedTimer > 0) {
                            targetX = gh->scatterX;
                            targetY = gh->scatterY;
                        } 
                        // Dacă nu sunt vulnerabile, funcționează logica clasică
                        else {
                            bool blinkyAngry = (gh->id == 0 && totalDots <= 30);
                            
                            if (globalMode == CHASE || blinkyAngry) {
                                if (gh->id == 0) {
                                    targetX = pacmanX; targetY = pacmanY;
                                } 
                                else if (gh->id == 2) {
                                    targetX = pacmanX + pacDirX * 2;
                                    targetY = pacmanY + pacDirY * 2;
                                    if (pacDirX == 0 && pacDirY == -1) targetX -= 2; 
                                } 
                                else if (gh->id == 1) {
                                    int refX = pacmanX + pacDirX * 2;
                                    int refY = pacmanY + pacDirY * 2;
                                    if (pacDirX == 0 && pacDirY == -1) refX -= 2; 
                                    targetX = refX + (refX - ghosts[0].x);
                                    targetY = refY + (refY - ghosts[0].y);
                                } 
                                else if (gh->id == 3) {
                                    int distToPac = GetDistanceSq(gh->x, gh->y, pacmanX, pacmanY);
                                    if (distToPac > 64) { 
                                        targetX = pacmanX; targetY = pacmanY;
                                    }
                                }
                            }
                        }

                        int bestDist = 999999;
                        int bestDirX = gh->dirX;
                        int bestDirY = gh->dirY;

                        int possibleDirs[4][2] = {{0,-1}, {-1,0}, {0,1}, {1,0}}; 

                        for (int i=0; i<4; i++) {
                            int dx = possibleDirs[i][0];
                            int dy = possibleDirs[i][1];

                            if (dx == -gh->dirX && dy == -gh->dirY) continue;

                            int nextX = gh->x + dx;
                            int nextY = gh->y + dy;

                            if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT && map[nextY][nextX] != 1) {
                                int dist = GetDistanceSq(nextX, nextY, targetX, targetY);
                                if (dist < bestDist) {
                                    bestDist = dist;
                                    bestDirX = dx;
                                    bestDirY = dy;
                                }
                            }
                        }

                        gh->dirX = bestDirX;
                        gh->dirY = bestDirY;
                        gh->x += bestDirX;
                        gh->y += bestDirY;

                        // --- VERIFICARE COLIZIUNE ACTUALIZATĂ ---
                        if (pacmanX == gh->x && pacmanY == gh->y) {
                            if (frightenedTimer > 0) {
                                // Pac-Man mănâncă fantoma!
                                score += 200;
                                gh->x = 13; // O reînviem în "casa fantomelor"
                                gh->y = 14;
                            } else {
                                // Fantoma mănâncă Pac-Man!
                                currentScreen = GAMEOVER; 
                            }
                        }
                    }
                    ghostFrameCounter = 0;
                }
                
                // Extra protecție pentru coliziuni între mișcările fantomelor (dacă trec una prin alta)
                for (int g = 0; g < 4; g++) {
                    if (pacmanX == ghosts[g].x && pacmanY == ghosts[g].y) {
                        if (frightenedTimer > 0) {
                            score += 200;
                            ghosts[g].x = 13;
                            ghosts[g].y = 14;
                        } else {
                            currentScreen = GAMEOVER;
                        }
                    }
                }
                break;
                
            case ENDSCREEN:
            case GAMEOVER:
                if (IsKeyPressed(KEY_ENTER)) { CloseWindow(); return 0; }
                break;
        }

        // --- 3. DESENARE ---
        BeginDrawing();
        ClearBackground(BLACK);

        switch(currentScreen) {
            case MENU:
                DrawText("PAC-MAN", screenWidth/2 - MeasureText("PAC-MAN", 60)/2, screenHeight/4, 60, YELLOW);
                Color buttonColor = CheckCollisionPointRec(GetMousePosition(), playButton) ? GRAY : DARKGRAY;
                DrawRectangleRec(playButton, buttonColor);
                DrawText("PLAY", playButton.x + 55, playButton.y + 15, 30, WHITE);
                break;

            case GAMEPLAY:
                for (int y = 0; y < MAP_HEIGHT; y++) {
                    for (int x = 0; x < MAP_WIDTH; x++) {
                        if (map[y][x] == 1) {
                            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBLUE);
                        }
                        else if (map[y][x] == 2) {
                            DrawCircle(x * TILE_SIZE + TILE_SIZE/2, y * TILE_SIZE + TILE_SIZE/2, 3, WHITE);
                        }
                        else if (map[y][x] == 3) {
                            // Desenăm cireașa mai mare și cu altă culoare pentru a o evidenția
                            DrawCircle(x * TILE_SIZE + TILE_SIZE/2, y * TILE_SIZE + TILE_SIZE/2, 6, RED);
                        }
                    }
                }

                DrawText(TextFormat("SCORE: %d | DOTS: %d", score, totalDots), 10, 10, 20, WHITE);

                DrawCircle(pacmanX * TILE_SIZE + TILE_SIZE / 2, pacmanY * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 2 - 2, YELLOW);

                // Desenăm cele 4 fantome, luând în calcul vulnerabilitatea
                for (int g = 0; g < 4; g++) {
                    // Dacă timer-ul e activ, le desenăm pe toate cu albastru închis, altfel folosesc culoarea originală
                    Color drawColor = (frightenedTimer > 0) ? DARKBLUE : ghosts[g].color;
                    DrawCircle(ghosts[g].x * TILE_SIZE + TILE_SIZE / 2, ghosts[g].y * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 2 - 2, drawColor);
                }
                break;
                
            case ENDSCREEN:
                DrawText("AI CASTIGAT!", screenWidth/2 - MeasureText("AI CASTIGAT!", 50)/2, screenHeight/2 - 50, 50, GREEN);
                DrawText(TextFormat("SCOR FINAL: %d", score), screenWidth/2 - MeasureText("SCOR FINAL: 0000", 30)/2, screenHeight/2 + 20, 30, WHITE);
                break;

            case GAMEOVER:
                DrawText("AI PIERDUT!", screenWidth/2 - MeasureText("AI PIERDUT!", 50)/2, screenHeight/2 - 50, 50, RED);
                DrawText(TextFormat("SCOR FINAL: %d", score), screenWidth/2 - MeasureText("SCOR FINAL: 0000", 30)/2, screenHeight/2 + 20, 30, WHITE);
                break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
