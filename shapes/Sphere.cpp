#include "Sphere.h"
#include <math.h>

Sphere::Sphere(int p1, int p2, float radius)
{
    setParamMax(2, 3, -1);
    setParam1(p1);
    setParam2(p2);

    m_radius = radius;
}


Sphere::~Sphere()
{
}


void Sphere::calcVerts()
{
    // 3 verts per triangle * slices *
    // (squares per slice - 1) * 2 * 2 for normals
    m_numVerts = 3 * m_p2 * ((m_p1 - 1) * 2) * 2;
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


void Sphere::make3Dslice(int *index, float thetaL, float thetaR)
{
    float phi = M_PI / m_p1;

    struct Rect rect;
    rect.tl = glm::vec3(m_radius * sin(phi) * cos(thetaL),
                        m_radius * cos(phi),
                        m_radius * sin(phi) * sin(thetaL));
    rect.tr = glm::vec3(m_radius * sin(phi) * cos(thetaR),
                        m_radius * cos(phi),
                        m_radius * sin(phi) * sin(thetaR));

    rect.tlNorm = glm::normalize(rect.tl);
    rect.trNorm = glm::normalize(rect.tr);

    // top triangle
    addVertex(index, glm::vec3(0, m_radius, 0), glm::vec3(0, 1, 0));
    addVertex(index, rect.tl, rect.tlNorm);
    addVertex(index, rect.tr, rect.trNorm);


    for (int i = 2; i < m_p1; i++) {
        phi = i * M_PI / m_p1;
        rect.bl = glm::vec3(m_radius * sin(phi) * cos(thetaL),
                            m_radius * cos(phi),
                            m_radius * sin(phi) * sin(thetaL));
        rect.br = glm::vec3(m_radius * sin(phi) * cos(thetaR),
                            m_radius * cos(phi),
                            m_radius * sin(phi) * sin(thetaR));

        rect.blNorm = glm::normalize(rect.bl);
        rect.brNorm = glm::normalize(rect.br);

        makeRect(index, &rect);

        rect.tl = rect.bl;
        rect.tr = rect.br;
        rect.trNorm = rect.brNorm;
        rect.tlNorm = rect.blNorm;
    }

    // bottom triangle
    addVertex(index, rect.tl, rect.tlNorm);
    addVertex(index, glm::vec3(0, -m_radius, 0), glm::vec3(0, -1, 0));
    addVertex(index, rect.tr, rect.trNorm);

}

