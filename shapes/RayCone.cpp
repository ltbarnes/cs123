#include "RayCone.h"

#define SLOPE 0.5f

RayCone::RayCone()
{
}


RayCone::~RayCone()
{
}


glm::vec4 RayCone::intersects(glm::vec4 p, glm::vec4 d)
{
    glm::vec4 n = glm::vec4(0, 0, 0, std::numeric_limits<float>::infinity());

    if (!this->intersectsAABB(p, d))
        return n;

    float horiz = p.x * p.x + p.z * p.z;
    float radius = (0.5f - p.y) * 0.5f;
    radius *= radius;
    float vert = std::abs(p.y);

    // if p is in the shape don't register an intersection
    if ((EQ(horiz, radius) || horiz < radius) && (EQ(vert, 0.5f) || vert < 0.5f))
        return n;

    float t1 = std::numeric_limits<float>::infinity();
    float t2 = std::numeric_limits<float>::infinity();
    float mag;
    glm::vec4 v;

    int tees = findT(p, d, &t1, &t2);

    if (tees) {
        v = p + t1 * d;
        if (v.y > 0.5 || v.y < -0.5 || t1 < 0)
            t1 = std::numeric_limits<float>::infinity();
        if (t1 < n.w) {
            mag = sqrt(v.x * v.x + v.z * v.z);
            n = glm::vec4(v.x, SLOPE * mag, v.z, t1);
        }
        if (tees == 2) {
            v = p + t2 * d;
            if (v.y > 0.5 || v.y < -0.5 || t2 < 0)
                t2 = std::numeric_limits<float>::infinity();
            if (t2 < n.w) {
                mag = sqrt(v.x * v.x + v.z * v.z);
                n = glm::vec4(v.x, SLOPE * mag, v.z, t2);
            }
        }
    }

    float t3 = (-0.5 - p.y) / d.y;
    v = p + t3 * d;

    if (v.x * v.x + v.z * v.z > 0.25f || t3 < 0)
        t3 = std::numeric_limits<float>::infinity();
    if (t3 < n.w) {
        n = glm::vec4(0, -1, 0, t3);
    }
    return n;
}


int RayCone::findT(glm::vec4 p, glm::vec4 d, float *t1, float *t2)
{
    double a = d.x * d.x + d.z * d.z - d.y * d.y / 4.0;
    double b = 2 * p.x * d.x + 2 * p.z * d.z - (2 * p.y * d.y - d.y) / 4.0;// + d.y or - d.y?
    double c = p.x * p.x + p.z * p.z - (p.y * p.y - p.y + .25) / 4.0;

    if (EQ(a, 0.0)) {
        *t1 = -c / b;
        return 1;
    }

    double disc = b * b - 4 * a * c;

    if (EQ(disc, 0.0)) {
        *t1 = -b / (2 * a);
        return 1;
    }
    if (disc < 0)
        return 0;

    *t1 = (-b - sqrt(disc)) / (2 * a);
    *t2 = (-b - sqrt(disc)) / (2 * a);
    return 2;
}


glm::vec2 RayCone::getUV(glm::vec4 point, glm::vec4 n)
{
    if (EQ(n.y, -1.f)) {
        return glm::vec2(point.x, point.z) + 0.5f;
    }

    float u = (std::atan2(-point.z, -point.x) + M_PI) / (2.f * M_PI);
    return glm::vec2(1.f - u, .5f + point.y);
}
