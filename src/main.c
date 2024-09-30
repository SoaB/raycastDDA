#include "ray.h"
#include "raylib.h"
#include "rcommon.h"
#include "vecmath.h"
#include <stdbool.h>

int Map[MAP_SIZE][MAP_SIZE] = { 0 };
Vector2 mousePosi = { 0 };
Vector2 mouseCell = { 0 };
Ray_s Player;

void Clip(Vector2* v, int x, int y, int w, int h)
{
    if (v->x < x)
        v->x = x;
    if (v->x > w)
        v->x = w;
    if (v->y < y)
        v->y = y;
    if (v->y > h)
        v->y = h;
}

void DrawMap()
{
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (Map[j][i] == 1) {
                DrawRectangle(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
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
    int angle = 0;
    Vector2 chkPos = { 0 };
    Vector2 chkPosCell = { 0 };
    mousePosi = GetMousePosition();
    Clip(&mousePosi, 0, 0, MAP_SIZE * CELL_SIZE - 1, MAP_SIZE * CELL_SIZE - 1);
    mouseCell = V2DivVal(mousePosi, CELL_SIZE);
    // set wall with mouse
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Map[(int)mouseCell.x][(int)mouseCell.y] = 1;
    }
    // update play position
    if (IsKeyDown(KEY_W)) {
        chkPos = V2Add(Player.pos, V2MulVal(Player.head, 5));
        chkPosCell = V2DivVal(chkPos, CELL_SIZE);
        if (Map[(int)chkPosCell.x][(int)chkPosCell.y] == 0)
            Player.pos = chkPos;
    }
    if (IsKeyDown(KEY_S)) {
        chkPos = V2Add(Player.pos, V2MulVal(Player.head, -5));
        chkPosCell = V2DivVal(chkPos, CELL_SIZE);
        if (Map[(int)chkPosCell.x][(int)chkPosCell.y] == 0)
            Player.pos = chkPos;
    }
    if (IsKeyDown(KEY_A)) {
        angle = 360 - 3;
        Player.head = V2Rotate(Player.head, Player.angle[angle]);
    }
    if (IsKeyDown(KEY_D)) {
        angle = 3;
        Player.head = V2Rotate(Player.head, Player.angle[angle]);
    }
    Clip(&Player.pos, 0, 0, MAP_SIZE * CELL_SIZE - 1, MAP_SIZE * CELL_SIZE - 1);
    Player.cell = V2DivVal(Player.pos, CELL_SIZE);
}
int main()
{
    Player = GenRay(28, 33);
    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Raycasting DDA");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateAll();
        BeginDrawing();
        ClearBackground((Color) { 18, 18, 18, 255 });
        DrawMap();
        DrawMouse();
        DrawRayPosition(&Player);
        RayLookWall(&Player);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}