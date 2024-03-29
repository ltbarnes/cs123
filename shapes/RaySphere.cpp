#include "RaySphere.h"

RaySphere::RaySphere()
{
}


RaySphere::~RaySphere()
{
}


glm::vec4 RaySphere::intersects(glm::vec4 p, glm::vec4 d)
{
    glm::vec4 n = glm::vec4(0, 0, 0, std::numeric_limits<float>::infinity());

    // check for bounding box intersection
    if (!this->intersectsAABB(p, d))
        return n;

    float t1 = std::numeric_limits<float>::infinity();
    float t2 = std::numeric_limits<float>::infinity();
    glm::vec4 v;

    // tees will only be 0, 1, or 2 (number of intersections)
    int tees = findT(p, d, &t1, &t2);

    if (tees) {
        v = p + t1 * d;
        if (v.y > 0.5 || v.y < -0.5 || t1 < 0)
            t1 = std::numeric_limits<float>::infinity();
        if (t1 < n.w) {
            v.w = t1;
            n = v;
        }
        if (tees == 2) {
            v = p + t2 * d;
            if (v.y > 0.5 || v.y < -0.5 || t2 < 0)
                t2 = std::numeric_limits<float>::infinity();
            if (t2 < n.w) {
                v.w = t2;
                n = v;
            }
        }
    }
    return n;
}


int RaySphere::findT(glm::vec4 p, glm::vec4 d, float *t1, float *t2)
{
    double a = d.x * d.x + d.y * d.y + d.z * d.z;
    double b = 2 * p.x * d.x + 2 * p.y * d.y + 2 * p.z * d.z;
    double c = p.x * p.x + p.y * p.y + p.z * p.z - 0.25;

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


glm::vec2 RaySphere::getUV(glm::vec4 point, glm::vec4 n)
{
    float y = std::atan2(glm::sqrt(n.x * n.x + n.z * n.z), n.y) / M_PI;
    float x = (std::atan2(-n.z, -n.x) + M_PI) / (2.f * M_PI);
    return glm::vec2(1.f - x, 1.f - y);
}

