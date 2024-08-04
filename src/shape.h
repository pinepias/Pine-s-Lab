#ifndef _SHAPE_H_
#define _SHAPE_H_

#define PI 3.14159265358979323846

#include "engine.h"
#include <stdio.h>

typedef struct Point                    Point;
typedef struct Rectangle                Rectangle;
typedef struct Polygon                  Polygon;
typedef struct Circle                   Circle;

typedef struct AABB                     AABB;
typedef struct SimpleCircle             SimpleCircle;

#define CIRCLE_POINTS_COUNT 62

struct Point
{
    float x, y;
};

struct Rectangle
{
    Point points[4];
};

struct Polygon
{
    Point *points;
    size_t count;
};

struct Circle
{
    Point points[62];
};

struct AABB
{
    float x, y, width, height;
};

struct SimpleCircle
{
    float x, y, radius;
};

Point Shape_NewPoint(float x, float y);
Point Shape_RotatePoint(Point point, Point origin, double rotation);

Rectangle Shape_NewRect(float x, float y, float width, float height, double rotation);

Polygon Shape_NewPolygon(Polygon *polygon, float x, float y, double rotation, int size, ...);
int Shape_GetPolygonArea(Point *points, int size);
Point Shape_GetPolygonCenter(Point *points, int size);
void Shape_FreePolygon(Polygon *polygon);

Circle Shape_NewCircle(float x, float y, float radius, double rotation);

AABB Shape_NewAABB(float x, float y, float width, float height);
SimpleCircle Shape_NewSimpleCircle(float x, float y, float radius);

void Shape_DebugRect(Rectangle rectangle, Color color, Window *window);
void Shape_DebugPolygon(Polygon polygon, Color color, Window *window);
void Shape_DebugCircle(Circle circle, Color color, Window *window);
void Shape_DebugAABBB(AABB aabb, Color color, Window *window);
void Shape_DebugSimpleCircle(SimpleCircle circle, Color color, Window *window);

#endif