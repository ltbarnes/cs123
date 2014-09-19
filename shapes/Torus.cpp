#include "Torus.h"
#include <math.h>

Torus::Torus(int p1, int p2, float p3, float radius)
{
    setParamMax(2, 3, 1.f);
    setParam1(p1);
    setParam2(p2);
    setParam3(p3);

    m_radius = radius;
}


Torus::~Torus()
{
}


void Torus::calcVerts()
{
    // 3 verts per triangle * slices *
    // (squares per slice - 1) * 2 * 2 for normals
    m_numVerts = 3 * m_p2 * ((m_p1) * 2) * 2;
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


void Torus::make3Dslice(int *index, float thetaL, float thetaR)
{
    float phi;
    float r = m_p3 / 100.f;

    struct Rect rect;
    rect.bl = glm::vec3((m_radius + r * cos(thetaL)) * cos(0),
                        (m_radius + r * cos(thetaL)) * sin(0),
                        r * sin(thetaL));
    rect.br = glm::vec3((m_radius + r * cos(thetaR)) * cos(0),
                        (m_radius + r * cos(thetaR)) * sin(0),
                        r * sin(thetaR));

    rect.blNorm = glm::normalize(rect.tl);
    rect.brNorm = glm::normalize(rect.tr);


    for (int i = 1; i <= m_p1; i++) {
        phi = i * M_PI * 2.f / m_p1;
        rect.tl = glm::vec3((m_radius + r * cos(thetaL)) * cos(phi),
                            (m_radius + r * cos(thetaL)) * sin(phi),
                            r * sin(thetaL));
        rect.tr = glm::vec3((m_radius + r * cos(thetaR)) * cos(phi),
                            (m_radius + r * cos(thetaR)) * sin(phi),
                            r * sin(thetaR));

        rect.tlNorm = glm::normalize(rect.bl);
        rect.trNorm = glm::normalize(rect.br);

        makeRect(index, &rect);

        rect.bl = rect.tl;
        rect.br = rect.tr;
        rect.brNorm = rect.trNorm;
        rect.blNorm = rect.tlNorm;
    }

}

