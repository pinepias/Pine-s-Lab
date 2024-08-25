#include "collision.h"
#include <SDL2/SDL_stdinc.h>

int FindClosestPolygonPoint(Vector2 center, Vector2 *vertices, int length);

bool IntersectCircles(Vector2 centerA, float radiusA, Vector2 centerB, float radiusB, 
                Vector2 *normal, float *depth)
{
    *normal = Vector2_Zero();
    *depth = __FLT_MAX__;

    float distance = Vector2_Distance(centerA, centerB);
    float raddi = radiusA + radiusB;

    if (distance >= raddi)
    {
        return false;
    }

    *normal = Vector2_Normalized(Vector2_Subtr(centerB, centerA));
    *depth = raddi - distance;

    return true;
}

bool IntersectPolygon(Vector2 *verticesA, int lengthA, Vector2 *verticesB, float lengthB,
                Vector2 *normal, float *depth)
{
    *normal = Vector2_Zero();
    *depth = __FLT_MAX__;

    for (int i = 0; i < lengthA; ++i)
    {
        Vector2 point0 = verticesA[i];
        Vector2 point1 = verticesA[(i + 1) % lengthA];

        Vector2 edge = Vector2_Subtr(point1, point0);
        Vector2 axis = Vector2_Normal(edge);
        axis = Vector2_Normalized(axis);

        float minA, maxA;
        float minB, maxB;

        Vector2_Projection(verticesA, lengthA, axis, &minA, &maxA);
        Vector2_Projection(verticesB, lengthB, axis, &minB, &maxB);

        if (minA > maxB || minB > maxA)
        {
            return false;
        }

        float axisDepth = SDL_min(maxA - minB, maxB - minA);

        if (axisDepth < *depth)
        {
            *depth = axisDepth;
            *normal = axis;
        }

    }

    for (int i = 0; i < lengthB; ++i)
    {
        Vector2 point0 = verticesB[i];
        Vector2 point1 = verticesB[(i + 1) % lengthA];

        Vector2 edge = Vector2_Subtr(point1, point0);
        Vector2 axis = Vector2_Normal(edge);
        axis = Vector2_Normalized(axis);

        float minA, maxA;
        float minB, maxB;

        Vector2_Projection(verticesA, lengthA, axis, &minA, &maxA);
        Vector2_Projection(verticesB, lengthB, axis, &minB, &maxB);

        

        if (minA > maxB || minB > maxA)
        {
            return false;
        }

        float axisDepth = SDL_min(maxA - minB, maxB - minA);

        if (axisDepth <= *depth)
        {
            *depth = axisDepth;
            *normal = axis;
        }
    }

    Vector2 centerA = PolygonGetCenter(verticesA, lengthA);
    Vector2 centerB = PolygonGetCenter(verticesB, lengthB);

    Vector2 direction = Vector2_Subtr(centerB, centerA);
    
    if (Vector2_Dot(direction, *normal) <= 0.0f)
    {
        *normal = Vector2_Mult(*normal, -1);
    }

    return true;
}

bool IntersectPolygonCircle(Vector2 center, int radius, Vector2 *vertices, int length,
                Vector2 *normal, float *depth)
{
    Vector2 axis = Vector2_Zero();
    *normal = Vector2_Zero();

    float axisDepth = __FLT_MAX__;
    *depth = __FLT_MAX__;

    float min, max;
    float cmin, cmax;

    for (int i = 0; i < length; ++i)
    {
        Vector2 point0 = vertices[i];
        Vector2 point1 = vertices[(i + 1) % length];

        Vector2 edge = Vector2_Subtr(point1, point0);
        axis = Vector2_Normal(edge);
        axis = Vector2_Normalized(axis);

        Vector2_Projection(vertices, length, axis, &min, &max);
        Vector2_ProjectionCircle(center, radius, axis, &cmin, &cmax);

        if (cmin > max || min > cmax)
        {
            return false;
        }

        axisDepth = SDL_min(cmax - min, max - cmin);

        if (axisDepth < *depth)
        {
            *depth = axisDepth;
            *normal = axis;
        }
    }

    int cpIndex = FindClosestPolygonPoint(center, vertices, length);
    Vector2 cp = vertices[cpIndex];

    axis = Vector2_Subtr(cp, center);
    axis = Vector2_Normalized(axis);

    Vector2_Projection(vertices, length, axis, &min, &max);
    Vector2_ProjectionCircle(center, radius, axis, &cmin, &cmax);

    if (min > cmax || cmin > max)
    {
        return false;
    }

    axisDepth = SDL_min(cmax - min, max - cmin);

    if (axisDepth < *depth)
    {
        *depth = axisDepth;
        *normal = axis;
    }

    Vector2 polygonCenter = PolygonGetCenter(vertices, length);
    Vector2 direction = Vector2_Subtr(polygonCenter, center);

    if (Vector2_Dot(direction, *normal) < 0.0f)
    {
        *normal = Vector2_Mult(*normal, -1);
    }

    return true;
}

float PolygonGetArea(Vector2 *vertices, int length)
{
    float result = 0;

    for (int i = 0; i < length; ++i)
    {
        Vector2 point0 = vertices[i];
        Vector2 point1 = vertices[(i + 1) % length];

        result = result + point0.x * point1.y - point1.x * point0.y;
    }

    return (result * 0.5);
}

Vector2 PolygonGetCenter(Vector2 *vertices, int length)
{
    float area = PolygonGetArea(vertices, length);
    float posX = 0;
    float posY = 0;

    for (int i = 0; i < length; ++i)
    {
        Vector2 point0 = vertices[i];
        Vector2 point1 = vertices[(i + 1) % length];

        posX = posX + (point0.x + point1.x) * (point0.x * point1.y - point1.x * point0.y);
        posY = posY + (point0.y + point1.y) * (point0.x * point1.y - point1.x * point0.y);
    }

    area = area * 6.0f;
    posX = posX / area;
    posY = posY / area;

    return Vector2_New(posX, posY);
}

int FindClosestPolygonPoint(Vector2 center, Vector2 *vertices, int length)
{
    int result = -1;
    float distance = __FLT_MAX__;

    for (int i = 0; i < length; ++i)
    {
        float tempDistance = Vector2_Distance(center, vertices[i]);

        if (tempDistance < distance)
        {
            result = i;
            distance = tempDistance;
        }
    }

    return result;
}