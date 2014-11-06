#include "AABB.h"

AABB::AABB()
{
}


AABB::~AABB()
{
}


void AABB::setMin(glm::vec3 min)
{
    m_min = min;
    if (m_maxSet)
        calcSA();
}


void AABB::setMax(glm::vec3 max)
{
    m_max = max;
    if (m_minSet)
        calcSA();
}


void AABB::calcSA()
{
    glm::vec3 l = m_max - m_min;
    m_sa = 2.f * l.x * l.y +
           2.f * l.x * l.z +
           2.f * l.y * l.z;
}


float AABB::getSA()
{
    return m_sa;
}


bool AABB::intersects(glm::vec4 p, glm::vec4 d)
{
    float tmin, tmax, tymin, tymax, txmin, txmax;
    glm::vec3 minMax[2] = {m_min, m_max};
    bool sign[3] = {d.x < 0, d.y < 0, d.z < 0};

    tmin = (minMax[sign[2]].z - p.z) / d.z;
    tmax = (minMax[!sign[2]].z - p.z) / d.z;
    tymin = (minMax[sign[1]].y - p.y) / d.y;
    tymax = (minMax[!sign[1]].y - p.y) / d.y;

    if (tmin > tymax || tymin > tmax)
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

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

