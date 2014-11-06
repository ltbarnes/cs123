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
