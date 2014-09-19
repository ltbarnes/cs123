#include "Torus.h"
#include <math.h>

Torus::Torus(int p1, int p2, float p3, float radius)
{
    setParamMax(3, 3, 1.f);
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
    // squares per slice * 2 * 2 for normals
    m_numVerts = 3 * m_p2 * m_p1 * 2 * 2;
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


void Torus::make3Dslice(int *index, float phiL, float phiR)
{
    float theta;
    float r = m_p3 / 100.f;
    glm::vec3 centerL = glm::vec3(cos(phiL), sin(phiL), 0) * m_radius;
    glm::vec3 centerR = glm::vec3(cos(phiR), sin(phiR), 0) * m_radius;

    struct Rect rect;
    rect.tl = glm::vec3((m_radius + r * cos(0)) * cos(phiL),
                        (m_radius + r * cos(0)) * sin(phiL),
                        r * sin(0));
    rect.tr = glm::vec3((m_radius + r * cos(0)) * cos(phiR),
                        (m_radius + r * cos(0)) * sin(phiR),
                        r * sin(0));

    rect.tlNorm = glm::normalize(rect.tl - centerL);
    rect.trNorm = glm::normalize(rect.tr - centerR);


    for (int i = 1; i <= m_p1; i++) {
        theta = i * M_PI * 2.f / m_p1;
        rect.bl = glm::vec3((m_radius + r * cos(theta)) * cos(phiL),
                            (m_radius + r * cos(theta)) * sin(phiL),
                            r * sin(theta));
        rect.br = glm::vec3((m_radius + r * cos(theta)) * cos(phiR),
                            (m_radius + r * cos(theta)) * sin(phiR),
                            r * sin(theta));

        rect.blNorm = glm::normalize(rect.bl - centerL);
        rect.brNorm = glm::normalize(rect.br - centerR);

        makeRect(index, &rect);

        rect.tl = rect.bl;
        rect.tr = rect.br;
        rect.trNorm = rect.brNorm;
        rect.tlNorm = rect.blNorm;
    }

}

