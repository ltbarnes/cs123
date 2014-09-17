#include "Cylinder.h"
#include <math.h>

Cylinder::Cylinder(int p1, int p2, float radius, float halfHeight)
    : Shape()
{
    setParamMax(1, 3, -1);
    setParam1(p1);
    setParam2(p2);

    m_radius = radius;
    m_halfHeight = halfHeight;
}


Cylinder::~Cylinder()
{

}


void Cylinder::calcVerts()
{
    // 3 verts per triangle * slices * (horizSquares * 2 +
    // (top and bottom squares * 2 - 1) * 2) * 2 for normals
//    m_numVerts = 3 * m_p2 * (m_p1 * 2 + ((m_p1 * 2) - 1) * 2) * 2;
    m_numVerts = 3 * m_p2 * ((m_p1 * 2) - 1) * 2 * 2;
    int size = m_numVerts * 3;
    m_vertexData = new GLfloat[size];

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
    for (int i = 1; i <= m_p2; i++) {
        curr[0] = x;
        curr[1] = z;

        make3DSlice(&index, curr, prev);

        t = x;
        x = cosine * x - sine * z;
        z = sine * t + cosine * z;

        prev[0] = curr[0];
        prev[1] = curr[1];
    }

}



void Cylinder::make3DSlice(int *index, glm::vec2 first, glm::vec2 second)
{
    // top circle
    makeCircleSlice(index, glm::vec3(first[0], m_halfHeight, first[1]),
            glm::vec3(second[0], m_halfHeight, second[1]), glm::vec3(0, 1, 0));

    // bottom circle
    makeCircleSlice(index, glm::vec3(second[0], -m_halfHeight, second[1]),
            glm::vec3(first[0], -m_halfHeight, first[1]), glm::vec3(0, -1, 0));

    // walls
//    for (int i = )
}


void Cylinder::makeCircleSlice(int *index, glm::vec3 first, glm::vec3 second, glm::vec3 norm)
{
    glm::vec3 tr = glm::vec3(second[0] / m_p1, second[1], second[2] / m_p1);
    glm::vec3 tl = glm::vec3(first[0] / m_p1, first[1], first[2] / m_p1);

    // add top triangle
    addVertex(index, norm * m_halfHeight, norm);
    addVertex(index, tl, norm);
    addVertex(index, tr, norm);

    glm::vec3 bl = glm::vec3(0, first[1], 0);
    glm::vec3 br = glm::vec3(0, second[1], 0);

    for (int i = 2; i <= m_p1; i++) {
        float scale = i * 1.f / m_p1;
        bl[0] = first[0] * scale;
        bl[2] = first[2] * scale;
        br[0] = second[0] * scale;
        br[2] = second[2] * scale;

        makeRect(index, tr, tl, bl, br, norm);

        tl = bl;
        tr = br;
    }
}

void Cylinder::makeRect(int *i, glm::vec3 tr, glm::vec3 tl, glm::vec3 bl, glm::vec3 br, glm::vec3 norm)
{
    addVertex(i, tr, norm);
    addVertex(i, tl, norm);
    addVertex(i, bl, norm);
    addVertex(i, bl, norm);
    addVertex(i, br, norm);
    addVertex(i, tr, norm);
}
