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
    // 3 verts per triangle * slices *
    // (bottom and cone squares * 2) - 1) * 2 for normals
    m_numVerts = 3 * m_p2 * ((m_p1 * 2) - 1) * 2 * 2;
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
    for (int i = 1; i <= m_p2; i++) {
        curr[0] = x;
        curr[1] = z;

        make3DSlice(&index, curr, prev, normSlope);

        t = x;
        x = cosine * x - sine * z;
        z = sine * t + cosine * z;

        prev[0] = curr[0];
        prev[1] = curr[1];
    }

}



void Cone::make3DSlice(int *index, glm::vec2 left, glm::vec2 right, float normSlope)
{
    // bottom circle
    makeCircleSlice(index, glm::vec3(right[0], -m_halfHeight, right[1]),
            glm::vec3(left[0], -m_halfHeight, left[1]), glm::vec3(0, -1, 0));

    struct Rect rect;

    // walls
    glm::vec3 spineL = glm::vec3(left[0], m_halfHeight * -2.f, left[1]) * (1.f / m_p1);
    glm::vec3 spineR = glm::vec3(right[0], m_halfHeight * -2.f, right[1]) * (1.f / m_p1);

    rect.tlNorm = glm::normalize(glm::vec3(left[0], m_radius * normSlope, left[1]));
    rect.trNorm = glm::normalize(glm::vec3(right[0], m_radius * normSlope, right[1]));
    rect.blNorm = rect.tlNorm;
    rect.brNorm = rect.trNorm;

    // point of cone
    glm::vec3 point = glm::vec3(0, m_halfHeight, 0);

    rect.bl = point + spineL;
    rect.br = point + spineR;

    // draw top triangle
    addVertex(index, point, glm::normalize(rect.tlNorm + rect.trNorm));
    addVertex(index, rect.bl, rect.tlNorm);
    addVertex(index, rect.br, rect.trNorm);

    for (int i = 1; i < m_p1; i++) {
        rect.tl = rect.bl;
        rect.tr = rect.br;
        rect.br = rect.tr + spineR;
        rect.bl = rect.tl + spineL;
        makeRect(index, &rect);
    }
}


void Cone::makeCircleSlice(int *index, glm::vec3 left, glm::vec3 right, glm::vec3 norm)
{
    struct Rect rect;
    rect.trNorm = norm;
    rect.tlNorm = norm;
    rect.blNorm = norm;
    rect.brNorm = norm;

    rect.tr = glm::vec3(right[0] / m_p1, right[1], right[2] / m_p1);
    rect.tl = glm::vec3(left[0] / m_p1, left[1], left[2] / m_p1);

    // add top triangle
    addVertex(index, norm * m_halfHeight, norm);
    addVertex(index, rect.tl, norm);
    addVertex(index, rect.tr, norm);

    rect.bl = glm::vec3(0, left[1], 0);
    rect.br = glm::vec3(0, right[1], 0);

    for (int i = 2; i <= m_p1; i++) {
        float scale = i * 1.f / m_p1;
        rect.bl[0] = left[0] * scale;
        rect.bl[2] = left[2] * scale;
        rect.br[0] = right[0] * scale;
        rect.br[2] = right[2] * scale;

        makeRect(index, &rect);

        rect.tl = rect.bl;
        rect.tr = rect.br;
    }
}
