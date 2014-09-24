#include "Cone.h"
#include <math.h>

Cone::Cone(int p1, int p2, float radius, float halfHeight)
    : Shape()
{
    setParamMax(1, 3, -1);
    setParam1(p1);
    setParam2(p2);

    m_radius = radius;
    m_halfHeight = halfHeight;
}


Cone::~Cone()
{
}


void Cone::calcVerts()
{
    m_numVerts = ((m_p2 * ((m_p1 + 1) * 2 * 2)) - 2) * 2;
    int size = m_numVerts * 3;
    m_vertexData = new GLfloat[size];

    // calc slope of normals
    float normSlope = m_radius / (m_halfHeight * 2.f);

    float angleSpacing = 2.f * M_PI / m_p2;
    float cosine = cos(angleSpacing);
    float sine = sin(angleSpacing);

    float x = m_radius;
    float z = 0;
    float t;

    glm::vec2 prev = glm::vec2(x, z);
    glm::vec2 curr = glm::vec2(0, 0);

    t = x;
    x = cosine * x - sine * z;
    z = sine * t + cosine * z;

    int index = 0;
    for (int i = 0; i < m_p2; i++) {
        curr[0] = x;
        curr[1] = z;

        make3DSlice(&index, curr, prev, normSlope);
        if (i != m_p2 - 1) {
            glm::vec3 nl = glm::normalize(glm::vec3(curr.x, m_radius * normSlope, curr.y));
            glm::vec3 nr = glm::normalize(glm::vec3(prev.x, m_radius * normSlope, prev.y));
            addVertex(&index, glm::vec3(0, -m_radius, 0), glm::vec3(0, -1, 0));
            addVertex(&index, glm::vec3(0, m_radius, 0), glm::normalize(nl + nr));
        }

        t = x;
        x = cosine * x - sine * z;
        z = sine * t + cosine * z;

        prev[0] = curr[0];
        prev[1] = curr[1];
    }

    if (size != index)
        cout << size << ", " << index << endl;

}



void Cone::make3DSlice(int *index, glm::vec2 left, glm::vec2 right, float normSlope)
{
    // walls
    glm::vec3 spineL = glm::vec3(left.x, m_halfHeight * -2.f, left.y) * (1.f / m_p1);
    glm::vec3 spineR = glm::vec3(right.x, m_halfHeight * -2.f, right.y) * (1.f / m_p1);

    glm::vec3 nl = glm::normalize(glm::vec3(left.x, m_radius * normSlope, left.y));
    glm::vec3 nr = glm::normalize(glm::vec3(right.x, m_radius * normSlope, right.y));

    // point of cone
    glm::vec3 point = glm::vec3(0, m_halfHeight, 0);
    addVertex(index, point, glm::normalize(nl + nr));

    for (int i = 1; i <= m_p1; i++) {

        addVertex(index, point + spineL * (1.f * i), nl);
        addVertex(index, point + spineR * (1.f * i), nr);
    }

    // bottom sliver
    makeBottomSlice(index, glm::vec3(left.x, -m_halfHeight, left.y), glm::vec3(right.x, -m_halfHeight, right.y));

    // ending point
    addVertex(index, glm::vec3(0, -m_halfHeight, 0), glm::vec3(0, -1, 0));


}


void Cone::makeBottomSlice(int *index, glm::vec3 left, glm::vec3 right)
{
    glm::vec3 n = glm::vec3(0, -1, 0);
    glm::vec3 vl = glm::vec3(0, left.y, 0);
    glm::vec3 vr = glm::vec3(0, right.y, 0);

    float scale;

    for (int i = m_p1; i > 0; i--) {
        scale = i * m_radius * 2.f / m_p1;
        vl.x = left.x * scale;
        vl.z = left.z * scale;
        vr.x = right.x * scale;
        vr.z = right.z * scale;

        addVertex(index, vl, n);
        addVertex(index, vr, n);
    }
}
