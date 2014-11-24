#include "RayCube.h"

RayCube::RayCube()
{
}


RayCube::~RayCube()
{
}


glm::vec4 RayCube::intersects(glm::vec4 p, glm::vec4 d)
{
    float tmin, tmax, tymin, tymax, txmin, txmax;
    glm::vec4 n;
    glm::vec3 minMax[2] = {glm::vec3(-0.5f), glm::vec3(0.5f)};
    bool sign[3] = {d.x < 0, d.y < 0, d.z < 0};

    tmin = (minMax[sign[2]].z - p.z) / d.z;
    tmax = (minMax[!sign[2]].z - p.z) / d.z;
    tymin = (minMax[sign[1]].y - p.y) / d.y;
    tymax = (minMax[!sign[1]].y - p.y) / d.y;

    if (tmin > tymax || tymin > tmax)
        return glm::vec4(0, 0, 0, std::numeric_limits<float>::infinity());

    if (tymin > tmin) {
        tmin = tymin;
        n = glm::vec4(0, (sign[1] ? 1 : -1), 0, tmin);
    } else
        n = glm::vec4(0, 0, (sign[2] ? 1 : -1), tmin);
    if (tymax < tmax)
        tmax = tymax;

    txmin = (minMax[sign[0]].x - p.x) / d.x;
    txmax = (minMax[!sign[0]].x - p.x) / d.x;

    if (tmin > txmax || txmin > tmax)
        return glm::vec4(0, 0, 0, std::numeric_limits<float>::infinity());

    if (txmin > tmin) {
        tmin = txmin;
        n = glm::vec4((sign[0] ? 1 : -1), 0, 0, tmin);
    }
    if (txmax < tmax)
        tmax = txmax;

    if (tmin > 0 && tmax < std::numeric_limits<float>::infinity())
        return n;
    else
        return glm::vec4(0, 0, 0, std::numeric_limits<float>::infinity());
}


glm::vec2 RayCube::getUV(glm::vec4 point, glm::vec4 n)
{
    n = glm::normalize(n);

    if (EQ(n.z, 1.f)) {
        glm::vec2 val = glm::vec2(point);
        return val + glm::vec2(0.5f);
    }
    if (EQ(n.z, -1.f)) {
        glm::vec2 val = glm::vec2(point);
        return glm::vec2(0.5f - val.x, val.y + 0.5f);
    }
    if (EQ(n.x, 1.f)) {
        glm::vec2 val = glm::vec2(point.z, point.y);
        return glm::vec2(0.5f - val.x, val.y + 0.5f);
    }
    if (EQ(n.x, -1.f)) {
        glm::vec2 val = glm::vec2(point.z, point.y);
        return val + glm::vec2(0.5f);
    }
    if (EQ(n.y, 1.f)) {
        glm::vec2 val = glm::vec2(point.x, point.z);
        return glm::vec2(0.5f + val.x, 0.5f - val.y);
    }
    if (EQ(n.y, -1.f)) {
        glm::vec2 val = glm::vec2(point.x, point.z);
        return val + glm::vec2(0.5f);
    }
    return glm::vec2(-1.f);
}


