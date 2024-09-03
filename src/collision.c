#include "collision.h"
#include <SDL2/SDL.h>

bool IntersectPolygon(Vector2 *verticesA, int lengthA, Vector2 *verticesB, int lengthB, 
                    Vector2 *normal, float *depth)
{
    *depth = FLT_MAX;
    Vector2_SetZero(normal);

    for (int i = 0; i < lengthA; ++i)
    {
        Vector2 point0 = {verticesA[i][0], verticesA[i][1]};
        Vector2 point1 = {verticesA[(i + 1) % lengthA][0], verticesA[(i + 1) % lengthA][1]};

        Vector2 edge;
        Vector2_Sub(&edge, point1, point0);
        Vector2 axis;
        Vector2_Normal(&axis, edge);
        Vector2_Normalizedl(&axis);

        float minA, minB;
        float maxA, maxB;
        
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
            Vector2_Setv(normal, axis);
        }
    }

    for (int i = 0; i < lengthB; ++i)
    {
        Vector2 point0 = {verticesB[i][0], verticesB[i][1]};
        Vector2 point1 = {verticesB[(i + 1) % lengthB][0], verticesB[(i + 1) % lengthB][1]};

        Vector2 edge;
        Vector2_Sub(&edge, point1, point0);
        Vector2 axis;
        Vector2_Normal(&axis, edge);
        Vector2_Normalizedl(&axis);

        float minA, minB;
        float maxA, maxB;
        
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
            Vector2_Setv(normal, axis);
        }
    }

    Vector2 centerA, centerB;
    PolygonGetCenter(&centerA, verticesA, lengthA);
    PolygonGetCenter(&centerB, verticesB, lengthB);

    Vector2 direction;
    Vector2_Sub(direction, centerB, centerA);

    if (Vector2_Dot(direction, *normal) < 0.0f)
    {
        Vector2_Multl(*normal, -1.0f);
    }

    return true;
}

float PolygonGetArea(Vector2 *vertices, int length)
{
    float sum = 0.0f;

    for (int i = 0; i < length; ++i)
    {
        Vector2 point0, point1;
        Vector2_Setv(&point0, vertices[i]);
        Vector2_Setv(&point1, vertices[(i + 1) % length]);
        sum = sum + (point0[0] * point1[1] - point1[0] * point0[1]);
    }

    sum = (sum * 0.5f);
    return sum;
}

void PolygonGetCenter(Vector2 *result, Vector2 *vertices, int length)
{
    float area = PolygonGetArea(vertices, length);
    float cx = 0.0f;
    float cy = 0.0f;

    for (int i = 0; i < length; ++i)
    {
        Vector2 point0, point1;
        Vector2_Setv(point0, vertices[i]);
        Vector2_Setv(point1, vertices[(i + 1) % length]);

        cx = cx + (point0[0] + point1[0]) * (point0[0] * point1[1] - point1[0] * point0[1]);
        cy = cy + (point0[1] + point1[1]) * (point0[0] * point1[1] - point1[0] * point0[1]);
    }

    area = area * 6.0f;
    cx = cx / area;
    cy = cy / area;

    Vector2_Set(result, cx, cy);
}

bool IntersectCircle(Vector2 *centerA, int radiusA, Vector2 *centerB, int radiusB, 
                    Vector2 *normal, float *depth)
{
    Vector2 dist;
    Vector2_Sub(&dist, centerB, centerA);
    float distance = Vector2_Length(dist);
    float raddi = radiusA + radiusB;

    if (distance > raddi)
    {
        return false;
    }

    Vector2_Sub(normal, centerB, centerA);
    Vector2_Normalizedl(normal);

    *depth = raddi - distance;

    return true;
}

bool IntersectPolygonCircle(Vector2 *vertices, int length, Vector2 *center, float radius, 
                    Vector2 *normal, float *depth)
{
    float axisDepth = FLT_MAX;
    *depth = FLT_MAX;

    Vector2 axis;
    Vector2_SetZero(&axis);
    Vector2_SetZero(normal);

    float min, max;
    float cmin, cmax;

    for (int i = 0; i < length; ++i)
    {
        Vector2 point0 = {vertices[i][0], vertices[i][1]};
        Vector2 point1 = {vertices[(i + 1) % length][0], vertices[(i + 1) % length][1]};

        Vector2 edge;
        Vector2_Sub(&edge, point1, point0);

        axis;
        Vector2_Normal(&axis, edge);
        Vector2_Normalizedl(&axis);

        Vector2_Projection(vertices, length, axis, &min, &max);
        Vector2_ProjectionCircle(center, radius, axis, &cmin, &cmax);

        if (min >= cmax || cmin >= max)
        {
            return false;
        }

        axisDepth = SDL_min(max - cmin, cmax - min);

        if (axisDepth < *depth)
        {
            *depth = axisDepth;
            Vector2_Setv(normal, axis);
        }
    }

    int cpIndex = FindClosestPointPolygon(center, vertices, length);

    Vector2 cp;
    Vector2_Setv(&cp, vertices[cpIndex]);

    Vector2_Sub(&axis, cp, center);
    Vector2_Normalizedl(&axis);

    Vector2_Projection(vertices, length, axis, &min, &max);
    Vector2_ProjectionCircle(center, radius, axis, &cmin, &cmax);

    if (min > cmax || cmin > max)
    {
        return false;
    }

    axisDepth = SDL_min(max - cmin, cmax - min);

    if (axisDepth < *depth)
    {
        *depth = axisDepth;
        Vector2_Setv(normal, axis);
    }

    Vector2 polygonCenter;
    PolygonGetCenter(&polygonCenter, vertices, length);

    Vector2 direction;
    Vector2_Sub(&direction, polygonCenter, center);

    if (Vector2_Dot(direction, *normal) < 0.0f)
    {
        Vector2_Multl(normal, -1.0f);
    }
}

int FindClosestPointPolygon(Vector2 center, Vector2 *vertices, int length)
{
    float realDistance = FLT_MAX;
    int index = -1;

    for (int i = 0; i < length; ++i)
    {
        Vector2 point = {vertices[i][0], vertices[i][1]};
        Vector2 dist;

        Vector2_Sub(&dist, point, center);
        float distance = Vector2_LengthSquared(dist);

        if (distance < realDistance)
        {
            index = i;
            realDistance = distance;
        }
    }

    return index;
}