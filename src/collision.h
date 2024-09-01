#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "types.h"
#include <stdbool.h>

bool IntersectPolygon(Vector2 *verticesA, int lengthA, Vector2 *verticesB, int lengthB, 
                    Vector2 *normal, float *depth);

bool IntersectCircle(Vector2 *centerA, int radiusA, Vector2 *centerB, int radiusB, 
                    Vector2 *normal, float *depth);

bool IntersectPolygonCircle(Vector2 *vertices, int length, Vector2 *center, int radius, 
                    Vector2 *normal, float *depth);

int FindClosestPointPolygon(Vector2 center, Vector2 *vertices, int length);

float PolygonGetArea(Vector2 *vertices, int length);
void PolygonGetCenter(Vector2 *result, Vector2 *vertices, int length);

#endif