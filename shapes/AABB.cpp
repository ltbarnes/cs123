#include "AABB.h"

AABB::AABB()
{
    m_min = glm::vec3(-1.f);
    m_max = glm::vec3(1.f);
    m_sa = calcSA(m_min, m_max);
}


AABB::~AABB()
{
}


void AABB::setMin(glm::vec3 min)
{
    m_min = min;
    m_sa = calcSA(m_min, m_max);
}


void AABB::setMax(glm::vec3 max)
{
    m_max = max;
    m_sa = calcSA(m_min, m_max);
}


glm::vec3 AABB::getMin()
{
    return m_min;
}


glm::vec3 AABB::getMax()
{
    return m_max;
}


float AABB::calcSA(const glm::vec3 min, const glm::vec3 max)
{
    glm::vec3 l = max - min;
    float sa = 2.f * l.x * l.y +
               2.f * l.x * l.z +
               2.f * l.y * l.z;
    return sa;
}


float AABB::getSA()
{
    return m_sa;
}


bool AABB::intersectsAABB(glm::vec4 p, glm::vec4 d)
{
    float tmin, tmax, tymin, tymax, txmin, txmax;
    glm::vec3 minMax[2] = {m_min, m_max};
    bool sign[3] = {d.x < 0, d.y < 0, d.z < 0};

    // check the interection distance to each plane
    tmin = (minMax[sign[2]].z - p.z) / d.z;
    tmax = (minMax[!sign[2]].z - p.z) / d.z;
    tymin = (minMax[sign[1]].y - p.y) / d.y;
    tymax = (minMax[!sign[1]].y - p.y) / d.y;

    // if the ray reaches a "side" plane before a "front" plane it doesn't intersect
    if (tmin > tymax || tymin > tmax)
        return false;

    // reset the min/max values
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    // check the intersection distance to the x plane
    txmin = (minMax[sign[0]].x - p.x) / d.x;
    txmax = (minMax[!sign[0]].x - p.x) / d.x;

    if (tmin > txmax || txmin > tmax)
        return false;

//    if (txmin > tmin)
//        tmin = txmin;
    if (txmax < tmax)
        tmax = txmax;

    return tmax > 0;
}

