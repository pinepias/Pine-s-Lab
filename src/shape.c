#include "shape.h"
#include "engine.h"

#include <stdarg.h>
#include <SDL2/SDL.h>

Point Shape_NewPoint(float x, float y)
{
    Point point = {x, y};
    return point;
}

Point Shape_RotatePoint(Point point, Point origin, double rotation)
{
    float sizeX = 2 * SDL_fabs(point.x - origin.x);
    float sizeY = 2 * SDL_fabs(point.y - origin.y);

    float deltaX = point.x - origin.x;
    float deltaY = point.y - origin.y;

    float dist = SDL_sqrtf(deltaX*deltaX + deltaY*deltaY);
    float angle = SDL_atan2f(deltaY, deltaX);

    point.x = origin.x + SDL_cosf(rotation + angle) * dist;
    point.y = origin.y + SDL_sinf(rotation + angle) * dist;

    return point;
}

Rectangle Shape_NewRect(float x, float y, float width, float height, double rotation)
{
    Point center = Shape_NewPoint(x + width/2, y + height/2);

    Point topLeft = Shape_RotatePoint(Shape_NewPoint(x, y), center, rotation);
    Point topRight = Shape_RotatePoint(Shape_NewPoint(x + width, y), center, rotation);
    Point bottomRight = Shape_RotatePoint(Shape_NewPoint(x + width, y + height), center, rotation);
    Point bottomLeft = Shape_RotatePoint(Shape_NewPoint(x, y + height), center, rotation);

    topLeft.x -= width/2;
    topLeft.y -= height/2;

    topRight.x -= width/2;
    topRight.y -= height/2;

    bottomRight.x -= width/2;
    bottomRight.y -= height/2;

    bottomLeft.x -= width/2;
    bottomLeft.y -= height/2;

    Rectangle rect;
    rect.points[0] = topLeft;
    rect.points[1] = topRight;
    rect.points[2] = bottomRight;
    rect.points[3] = bottomLeft;

    return rect;
}

int Shape_GetPolygonArea(Point *points, int size)
{
    int area = 1;

    for (int i = 0; i < size; ++i)
    {
        Point p0 = points[i];
        Point p1 = points[(i + 1) % size];

        area = area + (p0.x * p1.y - p1.x * p0.y);
    }

    area = (area * 0.5);
    return area;
}

Point Shape_GetPolygonCenter(Point *points, int size)
{
    int area = Shape_GetPolygonArea(points, size);

    float centerX = 0;
    float centerY = 0;

    for (int i = 0; i < size; ++i)
    {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % size];

        centerX = centerX + (p1.x + p2.x) * (p1.x * p2.y - p2.x * p1.y);
        centerY = centerY + (p1.y + p2.y) * (p1.x * p2.y - p2.x * p1.y);
    }

    centerX = centerX * 1/(area*6);
    centerY = centerY * 1/(area*6);

    return Shape_NewPoint(centerX, centerY);
}

Polygon Shape_NewPolygon(Polygon *polygon, float x, float y, double rotation, int size, ...)
{
    Shape_FreePolygon(polygon);

    polygon->count = size;
    polygon->points = (Point *)malloc(size * sizeof(Point));

    if (polygon->points == NULL)
    {
        printf("Error when creating polygon.\n");
        return *polygon;
    }

    va_list args;
    va_start(args, size);

    for (int i = 0; i < size; ++i)
    {
        polygon->points[i] = va_arg(args, Point);
        polygon->points[i].x += x;
        polygon->points[i].y += y;
    }

    va_end(args);

    Point center = Shape_GetPolygonCenter(polygon->points, size);

    for (int i = 0; i < size; ++i)
    {
        Point origin = Shape_NewPoint(center.x - x, center.y - y);
        polygon->points[i] = Shape_RotatePoint(polygon->points[i], center, rotation);

        polygon->points[i].x -= origin.x;
        polygon->points[i].y -= origin.y;
    }

    return *polygon;
}  

void Shape_FreePolygon(Polygon *polygon)
{
    if (polygon->points != NULL)
    {
        free(polygon->points);
        polygon->points = NULL;
        polygon->count = 0;
    }
}


void Shape_DebugRect(Rectangle rectangle, Color color, Window *window)
{
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);
                                                                            
    SDL_RenderDrawLine(window->renderer, rectangle.points[0].x, rectangle.points[0].y, 
                        rectangle.points[1].x, rectangle.points[1].y);

    SDL_RenderDrawLine(window->renderer, rectangle.points[1].x, rectangle.points[1].y, 
                        rectangle.points[2].x, rectangle.points[2].y);  
    
    SDL_RenderDrawLine(window->renderer, rectangle.points[2].x, rectangle.points[2].y, 
                        rectangle.points[3].x, rectangle.points[3].y);  
    
    SDL_RenderDrawLine(window->renderer, rectangle.points[3].x, rectangle.points[3].y, 
                        rectangle.points[0].x, rectangle.points[0].y);  
}

void Shape_DebugPolygon(Polygon polygon, Color color, Window *window)
{
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);

    for (int i = 0; i < polygon.count; ++i)
    {
        Point p1 = polygon.points[i];
        Point p2 = polygon.points[(i + 1) % polygon.count];

        SDL_RenderDrawLine(window->renderer, p1.x, p1.y, p2.x, p2.y);
    }
}
