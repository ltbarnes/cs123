#include "Cube.h"
#include <math.h>

Cube::Cube(int p1, float width)
    : Shape()
{
    m_p1 = p1;
    if (m_p1 < 1)
        m_p1 = 1;
    m_radius = width;

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

    int tbi, rli, oi, n;

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

    float spacing = m_radius * 2.f / m_p1;

    for (int r = 0; r < m_p1; r++) {
        for (int c = 0; c < m_p1; c++) {
            float right = (c + 1) * spacing - m_radius;
            float top = (r + 1) * spacing - m_radius;
            float left = c * spacing - m_radius;
            float bottom = r * spacing - m_radius;

            // top right
            tr[tbi] = top;
            tr[rli] = right;
            tr[oi] = m_radius * n;

            // top left
            tl[tbi] = top;
            tl[rli] = left;
            tl[oi] = m_radius * n;

            // bottom left
            bl[tbi] = bottom;
            bl[rli] = left;
            bl[oi] = m_radius * n;

            // bottom right
            br[tbi] = bottom;
            br[rli] = right;
            br[oi] = m_radius * n;

//            printVert(tr);
//            printVert(tl);
//            printVert(bl);
//            printVert(br);
            if (n > 0)
                makeSquare(index, tr, tl, bl, br, norm);
            else
                makeSquare(index, tr, br, bl, tl, norm);
        }
    }
}


void Cube::printVert(glm::vec3 v)
{
    cout << "(" << v.x;
    cout << ", " << v.y;
    cout << ", " << v.z;
    cout << ")" << endl;
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


