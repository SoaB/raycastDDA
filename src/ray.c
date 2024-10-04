#include "ray.h"
#include "global.h"
#define RAYMATH_IMPLEMENTATION
#include "raymath.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

extern int Map[MAP_SIZE][MAP_SIZE];
extern Color colmap[8];
extern Image imgTex[8];
extern uint32_t* pixels;
Ray_s GenRay(float x, float y)
{
    Ray_s r;
    r.pos.x = x;
    r.pos.y = y;
    r.dir = (Vector2) { 1, 0 };
    r.plane = (Vector2) { 0, 0.66 };
    return r;
}

void SetRayPos(Ray_s* r, Vector2 pos)
{
    r->pos = pos;
}
bool CastRay(Ray_s* r, HitWall* hw)
{
    // DDA Algorithm ==============================================
    // https://lodev.org/cgtutor/raycasting.html
    Vector2 rayStart = r->pos;
    Vector2 mapCheck = (Vector2) { (int)rayStart.x, (int)rayStart.y };
    // Lodev.org also explains this additional optimistaion (but it's beyond scope of video)
    Vector2 rayUnitStepSize = { fabsf(1.0f / r->rayDir.x), fabsf(1.0f / r->rayDir.y) };
    Vector2 rayLength1D = { 0 };
    Vector2 step = { 0 };
    // Establish Starting Conditions
    if (r->rayDir.x < 0) {
        step.x = -1;
        rayLength1D.x = (rayStart.x - mapCheck.x) * rayUnitStepSize.x;
    } else {
        step.x = 1;
        rayLength1D.x = ((mapCheck.x + 1) - rayStart.x) * rayUnitStepSize.x;
    }
    if (r->rayDir.y < 0) {
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
        Vector2 rayDirLen = Vector2Scale(r->rayDir, fDistance);
        hw->point.x = r->pos.x + rayDirLen.x;
        hw->point.y = r->pos.y + rayDirLen.y;
    }
    return bTileFound;
}
void DrawWall(Ray_s* r, HitWall* hw, int x)
{
    // Calculate height of line to draw on screen
    int lineHeight = (int)(SCR_HEIGHT / hw->distance);
    if (lineHeight < 1)
        lineHeight = 1;
    // calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + SCR_HEIGHT / 2;
    if (drawStart < 0)
        drawStart = 0;
    int drawEnd = lineHeight / 2 + SCR_HEIGHT / 2;
    if (drawEnd >= SCR_HEIGHT)
        drawEnd = SCR_HEIGHT - 1;

    // calculate value of wallX
    float wallX; // where exactly the wall was hit
    if (hw->side == 0)
        wallX = r->pos.y + hw->distance * r->rayDir.y;
    else
        wallX = r->pos.x + hw->distance * r->rayDir.x;
    wallX -= floorf(wallX);
    // x coordinate on the texture
    int texX = (int)(wallX * (float)TEX_WIDTH);
    if (hw->side == 0 && r->rayDir.x > 0)
        texX = TEX_WIDTH - texX - 1;
    if (hw->side == 1 && r->rayDir.y < 0)
        texX = TEX_WIDTH - texX - 1;
    int texNum = Map[(int)hw->hitCell.x][(int)hw->hitCell.y] - 1;
    uint32_t* pixs = (uint32_t*)imgTex[texNum].data;
    // TODO: an integer-only bresenham or DDA like algorithm
    // could make the texture coordinate stepping faster
    int st = (drawStart * SCR_WIDTH + x);
    for (int y = drawStart; y < drawEnd; y++) {
        // d := y*256 - SCR_HEIGHT*128 + lineHeight*128
        // 256 and 128 factors to avoid floats
        int d = (y << 8) - (SCR_HEIGHT << 7) + (lineHeight << 7);
        // TODO: avoid the division to speed this up
        int texY = ((d * TEX_SIZE) / lineHeight) / 256;
        int index = (TEX_SIZE * texY + texX);
        uint32_t da = pixs[index];
        // calc color for dark or something you want to change !
        if (hw->side) {
            da = ((da & 0x00FFFFFF) >> 1) | 0xFF000000;
        }
        pixels[st] = da;
        st += SCR_WIDTH;
    }
    // end of wall draw ...
    Vector2 floorWall;
    if (hw->side == 0 && r->rayDir.x > 0) {
        floorWall.x = hw->hitCell.x;
        floorWall.y = hw->hitCell.y + wallX;
    } else if (hw->side == 0 && r->rayDir.x < 0) {
        floorWall.x = hw->hitCell.x + 1.0;
        floorWall.y = hw->hitCell.y + wallX;
    } else if (hw->side == 1 && r->rayDir.y > 0) {
        floorWall.x = hw->hitCell.x + wallX;
        floorWall.y = hw->hitCell.y;
    } else {
        floorWall.x = hw->hitCell.x + wallX;
        floorWall.y = hw->hitCell.y + 1.0;
    }
    float distWall = hw->distance;
    float distPlayer = 0;
    st = ((drawEnd + 1) * SCR_WIDTH + x);
    int st1 = ((SCR_HEIGHT - drawEnd + 1) * SCR_WIDTH + x);
    Vector2 currentFloor;
    uint32_t* px1 = (uint32_t*)imgTex[6].data;
    uint32_t* px2 = (uint32_t*)imgTex[7].data;
    for (int y = drawEnd + 1; y < SCR_HEIGHT; y++) {
        float currentDist = (float)SCR_HEIGHT / (2.0 * (float)(y) - (float)(SCR_HEIGHT));
        float weight = (currentDist - distPlayer) / (distWall - distPlayer);
        currentFloor.x = weight * floorWall.x + (1.0 - weight) * r->pos.x;
        currentFloor.y = weight * floorWall.y + (1.0 - weight) * r->pos.y;
        int fx = (int)(currentFloor.x * (float)(TEX_SIZE)) % TEX_SIZE;
        int fy = (int)(currentFloor.y * (float)(TEX_SIZE)) % TEX_SIZE;
        int idx = (fy * TEX_SIZE + fx);
        // int idx = ((fy << 6) + fx);
        pixels[st] = px1[idx];
        pixels[st1] = px2[idx];
        st += SCR_WIDTH;
        st1 -= SCR_WIDTH;
    }
}
void RayLookWall(Ray_s* r)
{
    HitWall hw = { 0 };
    //    Color hcol = (Color) { 200, 200, 200, 150 };
    //    Color dcol = (Color) { 100, 100, 100, 100 };
    //    Color col;
    for (int x = 0; x < SCR_WIDTH; x++) {
        float cameraX = 2.0 * x / SCR_WIDTH - 1; // x-coordinate in camera space
        r->rayDir.x = r->dir.x + r->plane.x * cameraX;
        r->rayDir.y = r->dir.y + r->plane.y * cameraX;
        if (CastRay(r, &hw)) {
            //            if (hw.side)
            //                col = hcol;
            //            else
            //                col = dcol;
            //            Vector2 end = Vector2Scale(hw.point, CELL_SIZE);
            //            Vector2 st = Vector2Scale(r->pos, CELL_SIZE);
            //            DrawLineV(st, end, col);
            DrawWall(r, &hw, x);
        }
    }
}
void DrawRayPosition(Ray_s* r)
{
    Vector2 dest = Vector2Scale(r->dir, 4);
    Vector2 src = Vector2Scale(r->pos, CELL_SIZE);
    DrawCircleV(src, 4, YELLOW);
    dest = Vector2Add(src, dest);
    DrawLineV(src, dest, RED);
}
