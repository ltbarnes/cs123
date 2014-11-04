#include "RayCube.h"

RayCube::RayCube()
{
}


RayCube::~RayCube()
{
}


float RayCube::intersects(glm::vec4 p, glm::vec4 d)
{
    float bestT = std::numeric_limits<float>::infinity();
    float currT;
    glm::vec4 v;

    // top plane
    currT = (0.5 - p.y) / d.y;
    v = p + currT * d;

    if (v.x > 0.5 || v.x < -0.5 || v.z > 0.5f || v.z < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < bestT)
        bestT = currT;

    // bottom plane
    currT = (-0.5 - p.y) / d.y;
    v = p + currT * d;

    if (v.x > 0.5 || v.x < -0.5 || v.z > 0.5f || v.z < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < bestT)
        bestT = currT;

    // front plane
    currT = (0.5 - p.z) / d.z;
    v = p + currT * d;

    if (v.x > 0.5 || v.x < -0.5 || v.y > 0.5f || v.y < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < bestT)
        bestT = currT;

    // back plane
    currT = (-0.5 - p.z) / d.z;
    v = p + currT * d;

    if (v.x > 0.5 || v.x < -0.5 || v.y > 0.5f || v.y < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < bestT)
        bestT = currT;

    // right plane
    currT = (0.5 - p.x) / d.x;
    v = p + currT * d;

    if (v.y > 0.5 || v.y < -0.5 || v.z > 0.5f || v.z < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < bestT)
        bestT = currT;

    // left plane
    currT = (-0.5 - p.x) / d.x;
    v = p + currT * d;

    if (v.y > 0.5 || v.y < -0.5 || v.z > 0.5f || v.z < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < bestT)
        bestT = currT;

    return bestT;
}
