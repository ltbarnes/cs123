#include "RippleSphere.h"
#include <math.h>

RippleSphere::RippleSphere(int p1, int p2, float p3, float radius)
{
    setParamMax(2, 3, 1);
    setParam1(p1);
    setParam2(p2);
    setParam3(p3);

    m_radius = radius;
    m_depth = radius * .2f;
    m_rate = 1;
    m_ratef = M_PI * 2.f / 45.f;
}


RippleSphere::~RippleSphere()
{
}


void RippleSphere::calcVerts()
{
    // ((slices * verts per slice) - first and last vert) * 2 for normals
    m_numVerts = ((m_p2 * (m_p1 + 1) * 2) - 2) * 2;
    int size = m_numVerts * 3; // 3 points per vertex
    m_vertexData = new GLfloat[size];

    float prev = 0;
    float curr;

    int index = 0;
    glm::vec3 top = glm::vec3(0, m_radius * (1 + cos(m_ratef) * m_depth), 0);
    glm::vec3 bottom = glm::vec3(0, -m_radius * (1 + cos(M_PI * (m_p3 / 5.f) + m_ratef) * m_depth), 0);

    for (int i = 1; i <= m_p2; i++) {
        curr = i * M_PI * 2.f / m_p2;

        make3Dslice(&index, curr, prev, top, bottom);
        if (i != m_p2) {
            addVertex(&index, bottom, glm::vec3(0, -1, 0));
            addVertex(&index, top, glm::vec3(0, 1, 0));
        }

        prev = curr;
    }
}


void RippleSphere::make3Dslice(int *index, float thetaL, float thetaR, glm::vec3 top, glm::vec3 bottom)
{
    double spacing = M_PI / m_p1;
    float phi, a;
    glm::vec3 vl, vr;


    addVertex(index, top, glm::vec3(0, 1, 0));
    for (int i = 1; i < m_p1; i++) {
        phi = i * spacing;
        a = 1 + cos(phi * (m_p3 / 5.f) + (m_ratef)) * m_depth;

        vl = glm::vec3(m_radius * sin(phi) * cos(thetaL) * a,
                            m_radius * cos(phi) * a,
                            m_radius * sin(phi) * sin(thetaL) * a);
        vr = glm::vec3(m_radius * sin(phi) * cos(thetaR) * a,
                            m_radius * cos(phi) * a,
                            m_radius * sin(phi) * sin(thetaR) * a);

        addVertex(index, vl, calcNorm(phi, thetaL));
        addVertex(index, vr, calcNorm(phi, thetaR));
    }
    addVertex(index, bottom, glm::vec3(0, -1, 0));

}


bool RippleSphere::animate()
{
    if (--m_rate < 0)
        m_rate = 45;
    m_ratef = m_rate * M_PI * 2.f / 45.f;
    return true;
}


float RippleSphere::map(float value, float origRange, float newRange)
{
    return (value / origRange) * newRange;
}


glm::vec2 RippleSphere::rotate(glm::vec2 v, float angle)
{
    return glm::vec2(v.x * cos(angle) - v.y * sin(angle),
                     v.x * sin(angle) + v.y * cos(angle));
}


glm::vec3 RippleSphere::calcNorm(float phi, float theta)
{
    float tan = -sin(phi * (m_p3 / 5.f) + (m_ratef)) * (m_p3 / 5.f) * m_depth;
    glm::vec2 n = rotate(glm::vec2(-tan, 1), - phi);
    return glm::normalize(glm::vec3(cos(theta) * n.x, n.y, sin(theta) * n.x));
}
