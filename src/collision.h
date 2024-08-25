#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <stdbool.h>
#include "types.h"

bool IntersectCircles(Vector2 centerA, float radiusA, Vector2 centerB, float radiusB, 
                Vector2 *normal, float *depth);

bool IntersectPolygon(Vector2 *verticesA, int lengthA, Vector2 *verticesB, float lengthB,
                Vector2 *normal, float *depth);

bool IntersectPolygonCircle(Vector2 center, int radius, Vector2 *vertices, int length,
                Vector2 *normal, float *depth);

float PolygonGetArea(Vector2 *vertices, int length);
Vector2 PolygonGetCenter(Vector2 *vertices, int length);

#endif