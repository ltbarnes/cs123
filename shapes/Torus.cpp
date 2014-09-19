#include "Torus.h"
#include <math.h>

Torus::Torus(int p1, int p2, float p3, float radius)
{
    setParamMax(3, 3, 1.f);
    setParam1(p1);
    setParam2(p2);
    setParam3(p3);

    m_radius = radius;
    m_R = m_radius - (m_radius * m_p3 / 100.f);
    m_r = m_radius - m_R;
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


    m_R = m_radius - (m_radius * m_p3 / 100.f);
    m_r = m_radius - m_R;

    float prev = 0;
    float curr;

    int index = 0;
    for (int i = 1; i <= m_p1; i++) {
        curr = i * M_PI * 2.f / m_p1;

        make3Dslice(&index, curr, prev);

        prev = curr;
    }
}


void Torus::make3Dslice(int *index, float phiL, float phiR)
{
    float theta;
    glm::vec3 centerL = glm::vec3(cos(phiL), sin(phiL), 0) * m_R;
    glm::vec3 centerR = glm::vec3(cos(phiR), sin(phiR), 0) * m_R;

    struct Rect rect;
    rect.tl = glm::vec3((m_R + m_r * cos(0)) * cos(phiL),
                        (m_R + m_r * cos(0)) * sin(phiL),
                        m_r * sin(0));
    rect.tr = glm::vec3((m_R + m_r * cos(0)) * cos(phiR),
                        (m_R + m_r * cos(0)) * sin(phiR),
                        m_r * sin(0));

    rect.tlNorm = glm::normalize(rect.tl - centerL);
    rect.trNorm = glm::normalize(rect.tr - centerR);


    for (int i = 1; i <= m_p2; i++) {
        theta = i * M_PI * 2.f / m_p2;
        rect.bl = glm::vec3((m_R + m_r * cos(theta)) * cos(phiL),
                            (m_R + m_r * cos(theta)) * sin(phiL),
                            m_r * sin(theta));
        rect.br = glm::vec3((m_R + m_r * cos(theta)) * cos(phiR),
                            (m_R + m_r * cos(theta)) * sin(phiR),
                            m_r * sin(theta));

        rect.blNorm = glm::normalize(rect.bl - centerL);
        rect.brNorm = glm::normalize(rect.br - centerR);

        makeRect(index, &rect);

        rect.tl = rect.bl;
        rect.tr = rect.br;
        rect.trNorm = rect.brNorm;
        rect.tlNorm = rect.blNorm;
    }

}

