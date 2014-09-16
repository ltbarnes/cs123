#include "Cube.h"

Cube::Cube(int p1, float width)
    : Shape()
{
    m_p1 = p1;
    m_radius = width;

}

Cube::~Cube()
{
}


void Cube::calcVerts()
{
    // 6 sides * p1 x p1 squares * 6 vertices per square * 2 for normals
    m_numVerts = /*6 * */ m_p1 * m_p1 * 6 * 2;
    int size = m_numVerts * 3;
    m_vertexData = new GLfloat[size];

    int index = 0;
    makeSide(&index, glm::vec3(0, 0, 1));

}


void Cube::makeSide(int *index, glm::vec3 norm)
{
    float spacing = m_radius * 2.f / m_p1;
//    cout << spacing << endl;
    for (int r = 0; r < m_p1; r++) {
        for (int c = 0; c < m_p1; c++) {
            glm::vec3 tr = glm::vec3((c+1) * spacing - 0.5f, (r+1) * spacing - 0.5f, 0.5f);
            glm::vec3 tl = glm::vec3(c * spacing - 0.5f, (r+1) * spacing - 0.5f, 0.5f);
            glm::vec3 bl = glm::vec3(c * spacing - 0.5f, r * spacing - 0.5f, 0.5f);
            glm::vec3 br = glm::vec3((c+1) * spacing - 0.5f, r * spacing - 0.5f, 0.5f);
//            printVert(tr);
//            printVert(tl);
//            printVert(bl);
//            printVert(br);
            makeSquare(index, tr, tl, bl, br, norm);
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


