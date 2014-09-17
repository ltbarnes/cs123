#include "Cube.h"
#include <math.h>

Cube::Cube(int p1, float halfWidth)
    : Shape()
{
    setParamMax(1, -1, -1);
    setParam1(p1);
    m_halfWidth = halfWidth;

}

Cube::~Cube()
{
}


void Cube::calcVerts()
{
    // 6 sides * p1 x p1 squares * 6 vertices per square * 2 for normals
    m_numVerts = 6 * m_p1 * m_p1 * 6 * 2;
    int size = m_numVerts * 3;
    m_vertexData = new GLfloat[size];

    int index = 0;
    makeSide(&index, glm::vec3(0, 0, 1));
    makeSide(&index, glm::vec3(1, 0, 0));
    makeSide(&index, glm::vec3(0, 0, -1));
    makeSide(&index, glm::vec3(-1, 0, 0));
    makeSide(&index, glm::vec3(0, 1, 0));
    makeSide(&index, glm::vec3(0, -1, 0));
}


void Cube::makeSide(int *index, glm::vec3 norm)
{

    int tbi; // top and bottom index
    int rli; // right and left index
    int oi;  // other index
    int n;   // norm

    if (norm.x) {
        tbi = 2;
        rli = 1;
        oi = 0;
        n = norm.x;
    }
    else if (norm.y) {
        tbi = 0;
        rli = 2;
        oi = 1;
        n = norm.y;
    }
    else if (norm.z) {
        tbi = 1;
        rli = 0;
        oi = 2;
        n = norm.z;
    }

    glm::vec3 tr = glm::vec3();
    glm::vec3 tl = glm::vec3();
    glm::vec3 bl = glm::vec3();
    glm::vec3 br = glm::vec3();

    float spacing = m_halfWidth * 2.f / m_p1;

    for (int r = 0; r < m_p1; r++) {
        for (int c = 0; c < m_p1; c++) {
            float right = (c + 1) * spacing - m_halfWidth;
            float top = (r + 1) * spacing - m_halfWidth;
            float left = c * spacing - m_halfWidth;
            float bottom = r * spacing - m_halfWidth;

            // top right
            tr[tbi] = top;
            tr[rli] = right;
            tr[oi] = m_halfWidth * n;

            // top left
            tl[tbi] = top;
            tl[rli] = left;
            tl[oi] = m_halfWidth * n;

            // bottom left
            bl[tbi] = bottom;
            bl[rli] = left;
            bl[oi] = m_halfWidth * n;

            // bottom right
            br[tbi] = bottom;
            br[rli] = right;
            br[oi] = m_halfWidth * n;

            if (n > 0)
                makeSquare(index, tr, tl, bl, br, norm);
            else
                makeSquare(index, tr, br, bl, tl, norm);
        }
    }
}


void Cube::makeSquare(int *i, glm::vec3 tr, glm::vec3 tl, glm::vec3 bl, glm::vec3 br, glm::vec3 norm)
{
    addVertex(i, tr, norm);
    addVertex(i, tl, norm);
    addVertex(i, bl, norm);
    addVertex(i, bl, norm);
    addVertex(i, br, norm);
    addVertex(i, tr, norm);
}


