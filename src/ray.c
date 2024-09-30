#include "ray.h"
#include "rCommon.h"
#include "vecmath.h"
#include <raylib.h>
#include <stdbool.h>
extern int Map[MAP_SIZE][MAP_SIZE];
Ray_s GenRay(float x, float y)
{
    Ray_s r;
    r.pos.x = x;
    r.pos.y = y;
    r.dir = (Vector2) { 1, 0 };
    r.head = (Vector2) { 1, 0 };
    for (int i = 0; i < 360; i++) {
        r.angle[i] = i * DEG2RAD;
    }
    return r;
}

void SetRayPos(Ray_s* r, Vector2 pos)
{
    r->pos = pos;
}
static float f_abs(float a)
{
    if (a < 0)
        return -a;
    return a;
}
bool CastRay(Ray_s* r, HitWall* hw)
{
    // DDA Algorithm ==============================================
    // https://lodev.org/cgtutor/raycasting.html
    Vector2 rayStart = V2Clone(r->cell);
    Vector2 mapCheck = (Vector2) { (int)rayStart.x, (int)rayStart.y };

    // Lodev.org also explains this additional optimistaion (but it's beyond scope of video)
    Vector2 rayUnitStepSize = { f_abs(1.0f / r->dir.x), f_abs(1.0f / r->dir.y) };
    /*
    Vector2 rayUnitStepSize = {
        sqrt(1 + (r->dir.y * r->dir.y) / (r->dir.x * r->dir.x)),
        sqrt(1 + (r->dir.x * r->dir.x) / (r->dir.y * r->dir.y))
    };
    */
    Vector2 rayLength1D = { 0 };
    Vector2 step = { 0 };
    // Establish Starting Conditions
    if (r->dir.x < 0) {
        step.x = -1;
        rayLength1D.x = (rayStart.x - mapCheck.x) * rayUnitStepSize.x;
    } else {
        step.x = 1;
        rayLength1D.x = ((mapCheck.x + 1) - rayStart.x) * rayUnitStepSize.x;
    }
    if (r->dir.y < 0) {
        step.y = -1;
        rayLength1D.y = (rayStart.y - mapCheck.y) * rayUnitStepSize.y;
    } else {
        step.y = 1;
        rayLength1D.y = (mapCheck.y + 1 - rayStart.y) * rayUnitStepSize.y;
    }
    // Perform "Walk" until collision or range check
    float fMaxDistance = 30.0f;
    float fDistance = 0.0f;
    bool bTileFound = false;
    while (!bTileFound && fDistance < fMaxDistance) {
        // Walk along shortest path
        if (rayLength1D.x < rayLength1D.y) {
            mapCheck.x += step.x;
            fDistance = rayLength1D.x;
            rayLength1D.x += rayUnitStepSize.x;
            hw->side = 0;
        } else {
            mapCheck.y += step.y;
            fDistance = rayLength1D.y;
            rayLength1D.y += rayUnitStepSize.y;
            hw->side = 1;
        }
        // Test tile at new test point
        if (mapCheck.x >= 0 && mapCheck.x < MAP_SIZE && mapCheck.y >= 0 && mapCheck.y < MAP_SIZE) {
            //            if (Map[(int)mapCheck.x][(int)mapCheck.y] == 1) {
            //                bTileFound = true;
            //            }
            if (Map[(int)mapCheck.x][(int)mapCheck.y] == 1) {
                hw->hitCell = mapCheck;
                hw->distance = fDistance;
                bTileFound = true;
            }
        }
    }
    // Calculate intersection location
    if (bTileFound) {
        Vector2 rayDirLen = V2MulVal(r->dir, fDistance);
        hw->point.x = r->cell.x + rayDirLen.x;
        hw->point.y = r->cell.y + rayDirLen.y;
    }
    return bTileFound;
}
void DrawWall(Ray_s* r, HitWall* hw, int x)
{
    // Calculate height of line to draw on screen
    int lineHeight = (int)(SCR_HEIGHT / hw->distance);
    // calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + SCR_HEIGHT / 2;
    if (drawStart < 0)
        drawStart = 0;
    int drawEnd = lineHeight / 2 + SCR_HEIGHT / 2;
    if (drawEnd >= SCR_HEIGHT)
        drawEnd = SCR_HEIGHT - 1;

    // choose wall color
    Color color;
    switch (Map[(int)hw->hitCell.x][(int)hw->hitCell.y]) {
    case 1:
        color = RED;
        break; // red
    case 2:
        color = GREEN;
        break; // green
    case 3:
        color = BLUE;
        break; // blue
    case 4:
        color = WHITE;
        break; // white
    default:
        color = YELLOW;
        break; // yellow
    }
    // give x and y sides different brightness
    if (hw->side == 1) {
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
    }
    // draw the pixels of the stripe as a vertical line
    DrawLineEx((Vector2) { x, drawStart }, (Vector2) { x, drawEnd }, 8, color);
}
void RayLookWall(Ray_s* r)
{
    HitWall hw = { 0 };
    Color hcol = (Color) { 200, 200, 200, 150 };
    Color dcol = (Color) { 100, 100, 100, 100 };
    Color col;
    r->dir = V2Clone(r->head);
    r->dir = V2Rotate(r->dir, r->angle[330]);
    int startx = MAP_X_SIZE * CELL_SIZE + 10;
    for (int i = 0; i < 60; i++) {
        r->dir = V2Rotate(r->dir, r->angle[1]);
        r->dir = V2Norm(r->dir);
        if (CastRay(r, &hw)) {
            if (hw.side)
                col = hcol;
            else
                col = dcol;
            Vector2 end = V2MulVal(hw.point, CELL_SIZE);
            DrawLineV(r->pos, end, col);
            DrawWall(r, &hw, startx);
            startx += 8;
        }
    }
}
void DrawRayPosition(Ray_s* r)
{
    DrawCircleV(r->pos, 4, YELLOW);
    Vector2 dest;
    dest = V2Add(r->pos, V2MulVal(r->head, 20));
    DrawLineV(r->pos, dest, RED);
}
