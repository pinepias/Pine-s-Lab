#include "shape.h"
#include "collision.h"
#include "types.h"

bool Collision_DetectPolygon(Point *p0, int count0, Point *p1, int count1)
{
    for (int i = 0; i < count0; ++i)
    {
        Vector2 current = Vector2_New(p0[i].x, p0[i].y);
        Vector2 next = Vector2_New(p0[(i + 1) % count0].x, p0[(i + 1) % count0].y);

        Vector2 edge = Vector2_Subtr(next, current);
        Vector2 axis = Vector2_Normal(edge);

        float min0, min1;
        float max0, max1;

        Vector2_Project(p0, count0, axis, &min0, &max0);
        Vector2_Project(p1, count1, axis, &min1, &max1);

        if (min0 >= max1 || min1 >= max0)
        {
            return false;
        }
    }

    for (int i = 0; i < count1; ++i)
    {
        Vector2 current = Vector2_New(p1[i].x, p1[i].y);
        Vector2 next = Vector2_New(p1[(i + 1) % count1].x, p1[(i + 1) % count1].y);

        Vector2 edge = Vector2_Subtr(next, current);
        Vector2 axis = Vector2_Normal(edge);

        float min0, min1;
        float max0, max1;

        Vector2_Project(p0, count0, axis, &min0, &max0);
        Vector2_Project(p1, count1, axis, &min1, &max1);

        if (min0 >= max1 || min1 >= max0)
        {
            return false;
        }
    }

    return true;
}

bool Collision_DetectSimpleCircle(SimpleCircle c0, SimpleCircle c1)
{
    float dx = c1.x - c0.x;
    float dy = c1.y - c0.y;

    float dist = SDL_sqrtf((dx * dx) + (dy * dy));
    return dist <= (c0.radius + c1.radius);
}

bool Collision_DetectAABBB(AABB b0, AABB b1)
{
    return b0.x < b1.x + b1.width &&
            b0.x + b0.width > b1.x &&
            b0.y < b1.y + b1.height &&
            b0.y + b0.height > b1.y;
}

