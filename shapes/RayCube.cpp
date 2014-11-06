#include "RayCube.h"

RayCube::RayCube()
{
}


RayCube::~RayCube()
{
}


glm::vec4 RayCube::getNormal(glm::vec4 point)
{
    if (EQ(point.z, 0.5)) {
        return glm::vec4(0, 0, 1, 0);
    } if (EQ(point.z, -0.5)) {
        return glm::vec4(0, 0, -1, 0);
    } if (EQ(point.y, 0.5)) {
        return glm::vec4(0, 1, 0, 0);
    } if (EQ(point.y, -0.5)) {
        return glm::vec4(0, -1, 0, 0);
    } if (EQ(point.x, 0.5)) {
        return glm::vec4(1, 0, 0, 0);
    } if (EQ(point.x, -0.5)) {
        return glm::vec4(-1, 0, 0, 0);
    }
    return glm::vec4();
}


glm::vec4 RayCube::intersects(glm::vec4 p, glm::vec4 d)
{
    float currT;
    glm::vec4 v;
    glm::vec4 n = glm::vec4(0, 0, 0, std::numeric_limits<float>::infinity());

    // top plane
    currT = (0.5 - p.y) / d.y;
    v = p + currT * d;

    if (v.x > 0.5 || v.x < -0.5 || v.z > 0.5f || v.z < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < n.w) {
        n = glm::vec4(0, 1, 0, currT);
    }

    // bottom plane
    currT = (-0.5 - p.y) / d.y;
    v = p + currT * d;

    if (v.x > 0.5 || v.x < -0.5 || v.z > 0.5f || v.z < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < n.w) {
        n = glm::vec4(0, -1, 0, currT);
    }

    // front plane
    currT = (0.5 - p.z) / d.z;
    v = p + currT * d;

    if (v.x > 0.5 || v.x < -0.5 || v.y > 0.5f || v.y < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < n.w) {
        n = glm::vec4(0, 0, 1, currT);
    }

    // back plane
    currT = (-0.5 - p.z) / d.z;
    v = p + currT * d;

    if (v.x > 0.5 || v.x < -0.5 || v.y > 0.5f || v.y < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < n.w) {
        n = glm::vec4(0, 0, -1, currT);
    }

    // right plane
    currT = (0.5 - p.x) / d.x;
    v = p + currT * d;

    if (v.y > 0.5 || v.y < -0.5 || v.z > 0.5f || v.z < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < n.w) {
        n = glm::vec4(1, 0, 0, currT);
    }

    // left plane
    currT = (-0.5 - p.x) / d.x;
    v = p + currT * d;

    if (v.y > 0.5 || v.y < -0.5 || v.z > 0.5f || v.z < -0.5 || currT < 0)
        currT = std::numeric_limits<float>::infinity();
    if (currT < n.w) {
        n = glm::vec4(-1, 0, 0, currT);
    }

    return n;
}
