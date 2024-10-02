#ifndef __VEC_MATH_H__
#define __VEC_MATH_H__
#include <math.h>

#include "raylib.h"

Vector2 V2Zero(void);
Vector2 V2One(void);
int V2Equal(Vector2 v1, Vector2 v2);
Vector2 V2Set(float x, float y);
Vector2 V2Clone(Vector2 v);
Vector2 V2Add(Vector2 v1, Vector2 v2);
Vector2 V2AddVal(Vector2 v, float add);
Vector2 V2Sub(Vector2 v1, Vector2 v2);
Vector2 V2SubVal(Vector2 v, float sub);
float V2Length(Vector2 v);
float V2LengthSqr(Vector2 v);
float V2DotProduct(Vector2 v1, Vector2 v2);
float V2Distance(Vector2 v1, Vector2 v2);
float V2Angle(Vector2 v1, Vector2 v2);
Vector2 V2Scale(Vector2 v, float scale);
Vector2 V2Mul(Vector2 v1, Vector2 v2);
Vector2 V2MulVal(Vector2 v1, float val);
Vector2 V2Negate(Vector2 v);
Vector2 V2Div(Vector2 v1, Vector2 v2);
Vector2 V2DivVal(Vector2 v1, float val);
Vector2 V2Norm(Vector2 v);
Vector2 V2Lerp(Vector2 v1, Vector2 v2, float amount);
Vector2 V2Reflect(Vector2 v, Vector2 normal);
Vector2 V2Rotate(Vector2 v, float angle);
Vector2 V2MoveTowards(Vector2 v, Vector2 target, float maxDistance);
Vector2 V2Limit(Vector2 v, float n);
#endif
#ifdef __VEC_MATH_IMPLEMENTATION__
/////////////////////////////////////
Vector2 V2Zero(void)
{
    Vector2 result = { 0.0f, 0.0f };

    return result;
}

Vector2 V2One(void)
{
    Vector2 result = { 1.0f, 1.0f };

    return result;
}
int V2Equal(Vector2 v1, Vector2 v2)
{
    if (v1.x == v2.x && v1.y == v2.y)
        return 1;
    else
        return 0;
}
Vector2 V2Set(float x, float y)
{
    Vector2 result = { x, y };
    return result;
}

Vector2 V2Clone(Vector2 v)
{
    Vector2 result = { v.x, v.y };
    return result;
}

Vector2 V2Add(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x + v2.x, v1.y + v2.y };

    return result;
}

Vector2 V2AddVal(Vector2 v, float add)
{
    Vector2 result = { v.x + add, v.y + add };

    return result;
}

Vector2 V2Sub(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x - v2.x, v1.y - v2.y };

    return result;
}

Vector2 V2SubVal(Vector2 v, float sub)
{
    Vector2 result = { v.x - sub, v.y - sub };

    return result;
}

float V2Length(Vector2 v)
{
    float result = sqrtf((v.x * v.x) + (v.y * v.y));

    return result;
}

float V2LengthSqr(Vector2 v)
{
    float result = (v.x * v.x) + (v.y * v.y);

    return result;
}

float V2DotProduct(Vector2 v1, Vector2 v2)
{
    float result = (v1.x * v2.x + v1.y * v2.y);

    return result;
}

float V2Distance(Vector2 v1, Vector2 v2)
{
    float result = sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));

    return result;
}

float V2Angle(Vector2 v1, Vector2 v2)
{
    float result = atan2f(v2.y - v1.y, v2.x - v1.x) * (180.0f / PI);

    if (result < 0)
        result += 360.0f;

    return result;
}

Vector2 V2Scale(Vector2 v, float scale)
{
    Vector2 result = { v.x * scale, v.y * scale };

    return result;
}

Vector2 V2Mul(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x * v2.x, v1.y * v2.y };

    return result;
}
Vector2 V2MulVal(Vector2 v1, float val)
{
    Vector2 result = { v1.x * val, v1.y * val };

    return result;
}

Vector2 V2Negate(Vector2 v)
{
    Vector2 result = { -v.x, -v.y };

    return result;
}

Vector2 V2Div(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x / v2.x, v1.y / v2.y };

    return result;
}
Vector2 V2DivVal(Vector2 v1, float val)
{
    Vector2 result = { v1.x / val, v1.y / val };

    return result;
}

Vector2 V2Norm(Vector2 v)
{
    Vector2 result = { 0 };
    float length = sqrtf((v.x * v.x) + (v.y * v.y));

    if (length != 0) {
        result.x = v.x * 1.0f / length;
        result.y = v.y * 1.0f / length;
    }

    return result;
}

Vector2 V2Lerp(Vector2 v1, Vector2 v2, float amount)
{
    Vector2 result = { 0 };

    result.x = v1.x + amount * (v2.x - v1.x);
    result.y = v1.y + amount * (v2.y - v1.y);

    return result;
}

Vector2 V2Reflect(Vector2 v, Vector2 normal)
{
    Vector2 result = { 0 };

    float dotProduct = (v.x * normal.x + v.y * normal.y); // Dot product

    result.x = v.x - (2.0f * normal.x) * dotProduct;
    result.y = v.y - (2.0f * normal.y) * dotProduct;

    return result;
}

Vector2 V2Rotate(Vector2 v, float angle)
{
    Vector2 result = { 0 };

    result.x = v.x * cosf(angle) - v.y * sinf(angle);
    result.y = v.x * sinf(angle) + v.y * cosf(angle);

    return result;
}

Vector2 V2MoveTowards(Vector2 v, Vector2 target, float maxDistance)
{
    Vector2 result = { 0 };

    float dx = target.x - v.x;
    float dy = target.y - v.y;
    float value = (dx * dx) + (dy * dy);

    if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance * maxDistance)))
        return target;

    float dist = sqrtf(value);

    result.x = v.x + dx / dist * maxDistance;
    result.y = v.y + dy / dist * maxDistance;

    return result;
}
Vector2 V2Limit(Vector2 v, float n)
{
    Vector2 rv;
    rv.x = v.x;
    rv.y = v.y;
    float l = (v.x * v.x) + (v.y * v.y);
    if ((n * n) <= l) {
        l = sqrtf(l);
        float ra = n / l;
        rv.x = rv.x * ra;
        rv.y = rv.y * ra;
    }
    return rv;
}
#endif