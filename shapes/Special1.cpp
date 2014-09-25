#include "Special1.h"
#include <math.h>

Special1::Special1(int p1, int p2, float p3, float radius)
{
    setParamMax(3, 3, 1.f);
    setParam1(p1);
    setParam2(p2);
    setParam3(p3);

    m_radius = radius;
}


Special1::~Special1()
{
}


void Special1::calcVerts()
{
    // 3 verts per triangle * slices *
    // squares per slice * 2 * 2 for normals
    m_numVerts = 3 * m_p2 * m_p1 * 2 * 2 * 2;
    int size = m_numVerts * 3;
    m_vertexData = new GLfloat[size];

    float prev = 0;
    float curr;

    int index = 0;
    for (int i = 1; i <= m_p2; i++) {
        curr = i * M_PI * 2.f / m_p2;

        make3Dslice(&index, curr, prev);

        prev = curr;
    }
}


void Special1::make3Dslice(int *index, float phiL, float phiR)
{
    float theta;
    float r = m_p3 / 100.f;
    glm::vec3 centerL = glm::vec3(sin(phiL) * (exp(cos(phiL)) - 2 * cos(4 * phiL) - pow(sin(phiL/12.f), 5.f)),
                                  cos(phiL) * (exp(cos(phiL)) - 2 * cos(4 * phiL) - pow(sin(phiL/12.f), 5.f)),
                                  0) * m_radius;
    glm::vec3 centerR = glm::vec3(sin(phiR) * (exp(cos(phiR)) - 2 * cos(4 * phiR) - pow(sin(phiR/12.f), 5.f)),
                                  cos(phiR) * (exp(cos(phiR)) - 2 * cos(4 * phiR) - pow(sin(phiR/12.f), 5.f)),
                                  0) * m_radius;

}

