#include "ray.h"
#include "Common.h"
#include "vecmath.h"
#include <math.h>
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
bool CastRay(Ray_s* r, Vector2* p)
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
        } else {
            mapCheck.y += step.y;
            fDistance = rayLength1D.y;
            rayLength1D.y += rayUnitStepSize.y;
        }
        // Test tile at new test point
        if (mapCheck.x >= 0 && mapCheck.x < MAP_SIZE && mapCheck.y >= 0 && mapCheck.y < MAP_SIZE) {
            //            if (Map[(int)mapCheck.x][(int)mapCheck.y] == 1) {
            //                bTileFound = true;
            //            }
            if (Map[(int)mapCheck.x][(int)mapCheck.y] == 1) {
                bTileFound = true;
            }
        }
    }
    // Calculate intersection location
    if (bTileFound) {
        Vector2 rayDirLen = V2MulVal(r->dir, fDistance);
        p->x = r->cell.x + rayDirLen.x;
        p->y = r->cell.y + rayDirLen.y;
    }
    return bTileFound;
}
void RayLookWall(Ray_s* r)
{
    Vector2 cp;
    Color col = (Color) { 200, 200, 200, 150 };
    r->dir = V2Clone(r->head);
    for (int i = 0; i < 360; i++) {
        r->dir = V2Rotate(r->dir, r->angle[1]);
        r->dir = V2Norm(r->dir);
        if (CastRay(r, &cp)) {
            Vector2 end = V2MulVal(cp, CELL_SIZE);
            DrawLineV(r->pos, end, col);
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
