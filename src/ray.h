#ifndef __RAY_H__
#define __RAY_H__
#include "raylib.h"

typedef struct {
    Vector2 pos;
    Vector2 cell;
    Vector2 dir;
    Vector2 head;
} Ray_s;

typedef struct {
    Vector2 point;
    Vector2 hitCell;
    float distance;
    int side;
} HitWall;

Ray_s GenRay(float x, float y);
void SetRayPos(Ray_s* r, Vector2 pos);
bool CastRay(Ray_s* r, HitWall* p);
void RayLookWall(Ray_s* r);
void DrawRayPosition(Ray_s* r);
#endif