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
    int deadTimer; 
} Ghost;

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,3,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,3,1},
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
    {1,3,2,2,1,1,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,1,1,2,2,3,1},
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
    ghosts[0] = (Ghost){0, 13, 11, 1, 0, RED, MAP_WIDTH - 2, 1, 0};                     
    ghosts[1] = (Ghost){1, 11, 14, 1, 0, SKYBLUE, MAP_WIDTH - 2, MAP_HEIGHT - 2, 0};    
    ghosts[2] = (Ghost){2, 13, 14, 0, -1, MAGENTA, 1, 1, 0};                            
    ghosts[3] = (Ghost){3, 15, 14, -1, 0, ORANGE, 1, MAP_HEIGHT - 2, 0};                

    GhostMode globalMode = SCATTER;
    int modeTimer = 0; 
    int frightenedTimer = 0; 
    
    int score = 0; 
    int totalDots = 0;
    
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

                frameCounter++;
                if (frameCounter >= moveDelay) {
                    int nextX = pacmanX;
                    int nextY = pacmanY;

                    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { nextX++; pacDirX = 1; pacDirY = 0; }
                    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  { nextX--; pacDirX = -1; pacDirY = 0; }
                    else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    { nextY--; pacDirX = 0; pacDirY = -1; }
                    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  { nextY++; pacDirX = 0; pacDirY = 1; }

                    if (nextX < 0) nextX = MAP_WIDTH - 1;
                    else if (nextX >= MAP_WIDTH) nextX = 0;

                    if (nextY >= 0 && nextY < MAP_HEIGHT) {
                        if (map[nextY][nextX] != 1) {
                            pacmanX = nextX; pacmanY = nextY;
                            frameCounter = 0; 
                            
                            if (map[pacmanY][pacmanX] == 2) {
                                score += 10;                
                                map[pacmanY][pacmanX] = 0;  
                                totalDots--; 
                            }
                            else if (map[pacmanY][pacmanX] == 3) {
                                score += 50;
                                map[pacmanY][pacmanX] = 0;
                                totalDots--;
                                frightenedTimer = 600; 
                            }
                            
                            if (totalDots == 0) currentScreen = ENDSCREEN; 
                        }
                    }
                }

                ghostFrameCounter++;
                if (ghostFrameCounter >= ghostMoveDelay) {
                    
                    for (int g = 0; g < 4; g++) {
                        Ghost* gh = &ghosts[g];
                        
                        if (gh->deadTimer > 0) {
                            gh->deadTimer -= ghostMoveDelay; 
                            if (gh->deadTimer < 0) gh->deadTimer = 0;
                            continue; 
                        }
                        
                        int targetX = gh->scatterX;
                        int targetY = gh->scatterY;

                        if (frightenedTimer > 0) {
                            targetX = gh->scatterX;
                            targetY = gh->scatterY;
                        } 
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

                            if (nextX < 0) nextX = MAP_WIDTH - 1;
                            else if (nextX >= MAP_WIDTH) nextX = 0;

                            if (nextY >= 0 && nextY < MAP_HEIGHT && map[nextY][nextX] != 1) {
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
                        if (gh->x < 0) gh->x = MAP_WIDTH - 1;
                        else if (gh->x >= MAP_WIDTH) gh->x = 0;
                        
                        gh->y += bestDirY;

                        if (pacmanX == gh->x && pacmanY == gh->y && gh->deadTimer == 0) {
                            if (frightenedTimer > 0) {
                                score += 200;
                                gh->x = 13; 
                                gh->y = 14;
                                gh->deadTimer = 600; 
                            } else {
                                currentScreen = GAMEOVER; 
                            }
                        }
                    }
                    ghostFrameCounter = 0;
                }
                
                for (int g = 0; g < 4; g++) {
                    Ghost* gh = &ghosts[g];
                    if (pacmanX == gh->x && pacmanY == gh->y && gh->deadTimer == 0) {
                        if (frightenedTimer > 0) {
                            score += 200;
                            gh->x = 13;
                            gh->y = 14;
                            gh->deadTimer = 600; 
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
                            DrawCircle(x * TILE_SIZE + TILE_SIZE/2, y * TILE_SIZE + TILE_SIZE/2, 6, RED);
                        }
                    }
                }

                DrawText(TextFormat("SCORE: %d | DOTS: %d", score, totalDots), 10, 10, 20, WHITE);

                DrawCircle(pacmanX * TILE_SIZE + TILE_SIZE / 2, pacmanY * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 2 - 2, YELLOW);

                for (int g = 0; g < 4; g++) {
                    Color drawColor;
                    if (ghosts[g].deadTimer > 0) {
                        drawColor = LIGHTGRAY; 
                    } else if (frightenedTimer > 0) {
                        drawColor = DARKBLUE; 
                    } else {
                        drawColor = ghosts[g].color; 
                    }
                    
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