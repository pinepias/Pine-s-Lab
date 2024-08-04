#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <stdbool.h>

typedef struct Point               Point;

bool Collision_DetectPolygon(Point *p0, int count0, Point *p1, int count1);
bool Collision_DetectSimpleCircle(SimpleCircle c0, SimpleCircle c1);
bool Collision_DetectAABBB(AABB b0, AABB b1);

#endif