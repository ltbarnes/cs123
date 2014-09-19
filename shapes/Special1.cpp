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
    glm::vec3 centerL = glm::vec3(cos(phiL), sin(phiL), 0) * m_radius;
    glm::vec3 centerR = glm::vec3(cos(phiR), sin(phiR), 0) * m_radius;

    struct Rect rect;
    struct Rect hack;
    rect.tl = glm::vec3(sin(phiL) * (exp(cos(phiL)) - 2 * cos(4 * phiL) - pow(sin(phiL/12.f), 5.f) + r * cos(0)),
                        cos(phiL) * (exp(cos(phiL)) - 2 * cos(4 * phiL) - pow(sin(phiL/12.f), 5.f) + r * cos(0)),
                        r * sin(0));
    rect.tr = glm::vec3(sin(phiR) * (exp(cos(phiR)) - 2 * cos(4 * phiR) - pow(sin(phiR/12.f), 5.f) + r * cos(0)),
                        cos(phiR) * (exp(cos(phiR)) - 2 * cos(4 * phiR) - pow(sin(phiR/12.f), 5.f) + r * cos(0)),
                        r * sin(0));

    rect.tlNorm = glm::normalize(rect.tl - centerL);
    rect.trNorm = glm::normalize(rect.tr - centerR);


    for (int i = 1; i <= m_p1; i++) {
        theta = i * M_PI * 2.f / m_p1;
        rect.bl = glm::vec3(sin(phiL) * (exp(cos(phiL)) - 2 * cos(4 * phiL) - pow(sin(phiL/12.f), 5.f) + r * cos(theta)),
                            cos(phiL) * (exp(cos(phiL)) - 2 * cos(4 * phiL) - pow(sin(phiL/12.f), 5.f) + r * cos(theta)),
                            r * sin(theta));
        rect.br = glm::vec3(sin(phiR) * (exp(cos(phiR)) - 2 * cos(4 * phiR) - pow(sin(phiR/12.f), 5.f) + r * cos(theta)),
                            cos(phiR) * (exp(cos(phiR)) - 2 * cos(4 * phiR) - pow(sin(phiR/12.f), 5.f) + r * cos(theta)),
                            r * sin(theta));

        rect.blNorm = glm::normalize(rect.bl - centerL);
        rect.brNorm = glm::normalize(rect.br - centerR);

        hack.bl = rect.tl;
        hack.br = rect.tr;
        hack.tl = rect.bl;
        hack.tr = rect.br;
        hack.blNorm = rect.tlNorm;
        hack.brNorm = rect.trNorm;
        hack.trNorm = rect.brNorm;
        hack.blNorm = rect.blNorm;

        makeRect(index, &rect);
        makeRect(index, &hack);

        rect.tl = rect.bl;
        rect.tr = rect.br;
        rect.trNorm = rect.brNorm;
        rect.tlNorm = rect.blNorm;
    }

}

