#include "types.h"

void AABB_Set(AABB *aabb, float minX, float minY, float maxX, float maxY)
{
    Vector2_Set(&aabb[0], minX, minY);
    Vector2_Set(&aabb[1], maxX, maxY);
}

void AABB_Setv(AABB *aabb, Vector2 min, Vector2 max)
{
    Vector2_Setv(&aabb[0], min);
    Vector2_Setv(&aabb[1], max);
}