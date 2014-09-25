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
    m_numVerts = ((m_p2 * ((m_p1 * 2 * 3) + 6)) - 2) * 2;
    int size = m_numVerts * 3;
    m_vertexData = new GLfloat[size];

    // for rotation
    float angleSpacing = 2.f * M_PI / m_p2;
    float cosine = cos(angleSpacing);
    float sine = sin(angleSpacing);

    float x = m_radius;
    float z = 0;
    float temp;

    glm::vec2 prev = glm::vec2(x, z);
    glm::vec2 curr = glm::vec2(0, 0);

    // rotation matrix
    temp = x;
    x = cosine * x - sine * z;
    z = sine * temp + cosine * z;

    int index = 0;

    // iterate through the slices
    for (int i = 0; i < m_p2; i++) {
        curr[0] = x;
        curr[1] = z;

        make3DSlice(&index, curr, prev);

        // repeat the last point of this slice and the first point of the next
        // slice so the renderer won't connect the two points
        if (i != m_p2 - 1) {
            addVertex(&index, glm::vec3(0, -m_radius, 0), glm::vec3(0, -1, 0));
            addVertex(&index, glm::vec3(0, m_radius, 0), glm::vec3(0, 1, 0));
        }

        // rotation matrix
        temp = x;
        x = cosine * x - sine * z;
        z = sine * temp + cosine * z;

        prev[0] = curr[0];
        prev[1] = curr[1];
    }

}



void Cylinder::make3DSlice(int *index, glm::vec2 left, glm::vec2 right)
{
    // starting point
    addVertex(index, glm::vec3(0, m_halfHeight, 0), glm::vec3(0, 1, 0));

    // top sliver
    makeTopSlice(index, glm::vec3(left.x, m_halfHeight, left.y), glm::vec3(right.x, m_halfHeight, right.y));

    // make wall
    glm::vec3 nl = glm::vec3(left.x, 0, left.y);
    glm::vec3 nr = glm::vec3(right.x, 0, right.y);

    glm::vec3 vl = nl;
    glm::vec3 vr = nr;

    for (int i = m_p1; i >= 0; i--) {
        float y = (i * m_halfHeight * 2.f / m_p1) - m_halfHeight;
        vl.y = y;
        vr.y = y;

        addVertex(index, vl, nl);
        addVertex(index, vr, nr);
    }

    // bottom sliver
    makeBottomSlice(index, glm::vec3(left.x, -m_halfHeight, left.y), glm::vec3(right.x, -m_halfHeight, right.y));

    // ending point
    addVertex(index, glm::vec3(0, -m_halfHeight, 0), glm::vec3(0, -1, 0));
}


void Cylinder::makeTopSlice(int *index, glm::vec3 left, glm::vec3 right)
{
    glm::vec3 n = glm::vec3(0, 1, 0);
    glm::vec3 vl = glm::vec3(0, left.y, 0);
    glm::vec3 vr = glm::vec3(0, right.y, 0);

    float scale;

    // make top slice from center outward
    for (int i = 1; i <= m_p1; i++) {
        scale = i * m_radius * 2.f / m_p1;
        vl.x = left.x * scale;
        vl.z = left.z * scale;
        vr.x = right.x * scale;
        vr.z = right.z * scale;

        addVertex(index, vl, n);
        addVertex(index, vr, n);
    }
}


void Cylinder::makeBottomSlice(int *index, glm::vec3 left, glm::vec3 right)
{
    glm::vec3 n = glm::vec3(0, -1, 0);
    glm::vec3 vl = glm::vec3(0, left.y, 0);
    glm::vec3 vr = glm::vec3(0, right.y, 0);

    float scale;

    // make bottom slice moving inward toward the center
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
