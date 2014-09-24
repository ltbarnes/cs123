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
    // ((slices * verts per slice) - first and last vert) * 2 for normals
    m_numVerts = ((m_p2 * (m_p1 + 1) * 2) - 2) * 2;
    int size = m_numVerts * 3; // 3 points per vertex
    m_vertexData = new GLfloat[size];

    float prev = 0;
    float curr;

    int index = 0;

    for (int i = 1; i <= m_p2; i++) {
        curr = i * M_PI * 2.f / m_p2;

        make3Dslice(&index, curr, prev);
        if (i != m_p2) {
            addVertex(&index, glm::vec3(0, -m_radius, 0), glm::vec3(0, -1, 0));
            addVertex(&index, glm::vec3(0, m_radius, 0), glm::vec3(0, 1, 0));
        }

        prev = curr;
    }
}


void Sphere::make3Dslice(int *index, float thetaL, float thetaR)
{
    double spacing = M_PI / m_p1;
    float phi;

    addVertex(index, glm::vec3(0, m_radius, 0), glm::vec3(0, 1, 0));

    for (int i = 1; i < m_p1; i++) {
        phi = i * spacing;
        calcSliceSeg(index, thetaL, thetaR, phi);
    }
    addVertex(index, glm::vec3(0, -m_radius, 0), glm::vec3(0, -1, 0));
}

void Sphere::calcSliceSeg(int *index, float thetaL, float thetaR, float phi)
{
    glm::vec3 vl = glm::vec3(m_radius * sin(phi) * cos(thetaL),
                        m_radius * cos(phi),
                        m_radius * sin(phi) * sin(thetaL));
    glm::vec3 vr = glm::vec3(m_radius * sin(phi) * cos(thetaR),
                        m_radius * cos(phi),
                        m_radius * sin(phi) * sin(thetaR));

    addVertex(index, vl, glm::normalize(vl));
    addVertex(index, vr, glm::normalize(vr));
}

