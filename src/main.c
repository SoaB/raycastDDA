#include "global.h"
#include "ray.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <stdint.h>

int Map[MAP_SIZE][MAP_SIZE] = {
    { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7 },
    { 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7 },
    { 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7 },
    { 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7 },
    { 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7 },
    { 4, 0, 4, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 0, 7, 7, 7, 7, 7 },
    { 4, 0, 5, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1 },
    { 4, 0, 6, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8 },
    { 4, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 1 },
    { 4, 0, 8, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8 },
    { 4, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1 },
    { 4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 1 },
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },
    { 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 6, 0, 6, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3 },
    { 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2 },
    { 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2 },
    { 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2 },
    { 4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 2 },
    { 4, 0, 0, 5, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2 },
    { 4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2 },
    { 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2 },
    { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3 }
};
Vector2 mousePosi = { 0 };
Vector2 mouseCell = { 0 };
Ray_s Player;
Image imgTex[8];
Image buffer;
uint32_t* pixels;
Texture2D texView;
Color colmap[8] = {
    WHITE, RED, GREEN, BLUE, GOLD, PINK, MAROON, PURPLE
};

void GenTexture()
{
    imgTex[0] = LoadImage("pics/eagle.png");
    imgTex[1] = LoadImage("pics/redbrick.png");
    imgTex[2] = LoadImage("pics/purplestone.png");
    imgTex[3] = LoadImage("pics/greystone.png");
    imgTex[4] = LoadImage("pics/bluestone.png");
    imgTex[5] = LoadImage("pics/mossy.png");
    imgTex[6] = LoadImage("pics/wood.png");
    imgTex[7] = LoadImage("pics/colorstone.png");
    for (int i = 0; i < 8; i++) {
        ImageFormat(&imgTex[i], 7);
    }
}
void FiniTex()
{
    for (int i = 0; i < 8; i++) {
        UnloadImage(imgTex[i]);
    }
}
void DrawMap()
{
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (Map[j][i] > 0) {
                DrawRectangle(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, colmap[3]);
            } else
                DrawRectangleLines(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, (Color) { 66, 66, 66, 255 });
        }
    }
}
void DrawMouse()
{
    int x = (int)mouseCell.x * CELL_SIZE;
    int y = (int)mouseCell.y * CELL_SIZE;
    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, RED);
}
void UpdateAll()
{
    float frameTime = GetFrameTime();
    float moveSpeed = frameTime * 6.0; // the constant value is in squares/second
    float rotSpeed = frameTime * 3.0; // the constant value is in radians/second
    // move forward if no wall in front of you
    if (IsKeyDown(KEY_W)) {
        if (Map[(int)(Player.pos.x + Player.dir.x * moveSpeed)][(int)Player.pos.y] == 0) {
            Player.pos.x += Player.dir.x * moveSpeed;
        }
        if (Map[(int)(Player.pos.x)][(int)(Player.pos.y + Player.dir.y * moveSpeed)] == 0) {
            Player.pos.y += Player.dir.y * moveSpeed;
        }
    }
    // move backwards if no wall behind you
    if (IsKeyDown(KEY_S)) {
        if (Map[(int)(Player.pos.x - Player.dir.x * moveSpeed)][(int)(Player.pos.y)] == 0) {
            Player.pos.x -= Player.dir.x * moveSpeed;
        }
        if (Map[(int)(Player.pos.x)][(int)(Player.pos.y - Player.dir.y * moveSpeed)] == 0) {
            Player.pos.y -= Player.dir.y * moveSpeed;
        }
    }
    if (IsKeyDown(KEY_A)) {
        Player.dir = Vector2Rotate(Player.dir, -rotSpeed);
        Player.plane = Vector2Rotate(Player.plane, -rotSpeed);
    }
    if (IsKeyDown(KEY_D)) {
        Player.dir = Vector2Rotate(Player.dir, rotSpeed);
        Player.plane = Vector2Rotate(Player.plane, rotSpeed);
    }
}
void InitDrawBuffer()
{
    Image buffer = GenImageColor(SCR_WIDTH, SCR_HEIGHT, BLACK);
    texView = LoadTextureFromImage(buffer);
    pixels = (uint32_t*)buffer.data;
}
void ClearPixels(uint32_t color)
{
    for (int i = 0; i < buffer.width * buffer.height; i++) {
        pixels[i] = color;
    }
}
void DrawBuffer()
{
    ClearPixels(0xFF000000);
    RayLookWall(&Player);
    UpdateTexture(texView, (void*)pixels);
    DrawTexture(texView, 0, 0, WHITE);
    DrawMap();
    //    DrawMouse();
    DrawRayPosition(&Player);
}
int main()
{
    Player = GenRay(1, 1);
    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Raycasting DDA");
    InitDrawBuffer();
    GenTexture();
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateAll();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawBuffer();
        EndDrawing();
    }
    UnloadImage(buffer);
    UnloadTexture(texView);
    FiniTex();
    CloseWindow();
    return 0;
}