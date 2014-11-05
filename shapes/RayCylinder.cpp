#include "RayCylinder.h"

RayCylinder::RayCylinder()
{
}


RayCylinder::~RayCylinder()
{
}


glm::vec4 RayCylinder::getNormal(glm::vec4 point)
{
    if (EQ(point.y, 0.5)) {
//        assert(point.x * point.x + point.z * point.z <= 0.5f);
        return glm::vec4(0, 1, 0, 0);
    }
    if (EQ(point.y, -0.5)) {
//        assert(point.x * point.x + point.z * point.z <= 0.5f);
        return glm::vec4(0, -1, 0, 0);
    }
    else {
//        assert(EQ(point.x * point.x + point.z * point.z, 0.5f));
        return glm::vec4(point.x, 0, point.z, 0);
    }
    return glm::vec4();
}


float RayCylinder::intersects(glm::vec4 p, glm::vec4 d)
{
    float t1 = std::numeric_limits<float>::infinity();
    float t2 = std::numeric_limits<float>::infinity();
    glm::vec4 v;

    int tees = findT(p, d, &t1, &t2);

    if (tees) {
        v = p + t1 * d;
        if (v.y > 0.5 || v.y < -0.5 || t1 < 0)
            t1 = std::numeric_limits<float>::infinity();
        if (tees == 2) {
            v = p + t2 * d;
            if (v.y > 0.5 || v.y < -0.5 || t2 < 0)
                t2 = std::numeric_limits<float>::infinity();
        }
    }

    float t3 = (0.5 - p.y) / d.y;
    v = p + t3 * d;

    if (v.x * v.x + v.z * v.z > 0.25f || t3 < 0)
        t3 = std::numeric_limits<float>::infinity();

    float t4 = (-0.5 - p.y) / d.y;
    v = p + t4 * d;

    if (v.x * v.x + v.z * v.z > 0.25f || t4 < 0)
        t4 = std::numeric_limits<float>::infinity();

    return std::min(t1, std::min(t2, std::min(t3, t4)));
}

int RayCylinder::findT(glm::vec4 p, glm::vec4 d, float *t1, float *t2)
{
    double a = d.x * d.x + d.z * d.z;
    double b = 2 * p.x * d.x + 2 * p.z * d.z;
    double c = p.x * p.x + p.z * p.z - 0.25;

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
