#ifndef __RAY_H__
#define __RAY_H__
#include "raylib.h"

typedef struct {
    Vector2 pos;
    Vector2 cell;
    Vector2 dir;
    Vector2 head;
    float angle[360];
} Ray_s;

Ray_s GenRay(float x, float y);
void SetRayPos(Ray_s* r, Vector2 pos);
bool CastRay(Ray_s* r, Vector2* p);
void RayLookWall(Ray_s* r);
void DrawRayPosition(Ray_s* r);
#endif