#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "engine.h"
#include <stdio.h>

typedef struct Point                    Point;
typedef struct Rectangle                Rectangle;
typedef struct Polygon                  Polygon;

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

Point Shape_NewPoint(float x, float y);
Point Shape_RotatePoint(Point point, Point origin, double rotation);

Rectangle Shape_NewRect(float x, float y, float width, float height, double rotation);

Polygon Shape_NewPolygon(Polygon *polygon, float x, float y, double rotation, int size, ...);

int Shape_GetPolygonArea(Point *points, int size);
Point Shape_GetPolygonCenter(Point *points, int size);

void Shape_FreePolygon(Polygon *polygon);

void Shape_DebugRect(Rectangle rectangle, Color color, Window *window);
void Shape_DebugPolygon(Polygon polygon, Color color, Window *window);

#endif