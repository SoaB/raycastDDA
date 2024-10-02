#include "ray.h"
#include "global.h"
#include "vecmath.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
extern int Map[MAP_SIZE][MAP_SIZE];
extern Color colmap[8];
uint32_t TexArr[8][TEX_SIZE * TEX_SIZE];
void GenTexture()
{
    for (int x = 0; x < TEX_SIZE; x++)
        for (int y = 0; y < TEX_SIZE; y++) {
            int xorcolor = (x * 256 / TEX_SIZE) ^ (y * 256 / TEX_SIZE);
            // int xcolor = x * 256 / texWidth;
            int ycolor = y * 256 / TEX_SIZE;
            int xycolor = y * 128 / TEX_SIZE + x * 128 / TEX_SIZE;
            TexArr[0][TEX_SIZE * y + x] = 65536 * 254 * (x != y && x != TEX_SIZE - y); // flat red texture with black cross
            TexArr[1][TEX_SIZE * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; // sloped greyscale
            TexArr[2][TEX_SIZE * y + x] = 256 * xycolor + 65536 * xycolor; // sloped yellow gradient
            TexArr[3][TEX_SIZE * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; // xor greyscale
            TexArr[4][TEX_SIZE * y + x] = 256 * xorcolor; // xor green
            TexArr[5][TEX_SIZE * y + x] = 65536 * 192 * (x % 16 && y % 16); // red bricks
            TexArr[6][TEX_SIZE * y + x] = 65536 * ycolor; // red gradient
            TexArr[7][TEX_SIZE * y + x] = 128 + 256 * 128 + 65536 * 128; // flat grey texture
        }
}
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
    GenTexture();
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
            if (Map[(int)mapCheck.x][(int)mapCheck.y] >= 1) {
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
    int pitch = 100;
    // calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + SCR_HEIGHT / 2 + pitch;
    if (drawStart < 0)
        drawStart = 0;
    int drawEnd = lineHeight / 2 + SCR_HEIGHT / 2 + pitch;
    if (drawEnd >= SCR_HEIGHT)
        drawEnd = SCR_HEIGHT - 1;

    // choose wall color
    Color color;
    color = colmap[Map[(int)hw->hitCell.x][(int)hw->hitCell.y]];
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
    int startx = MAP_X_SIZE * CELL_SIZE;
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
