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
    // 6 sides * 2w verts * p1 strips * 2 for normals
    int w = m_p1 + 1;
    m_numVerts = (6 * ((2 * w + 2) * m_p1) - 2) * 2; // for normals
    int size = m_numVerts * 3;
    m_vertexData = new GLfloat[size];

    double spacing = m_halfWidth * 2.0 / m_p1;

    int index = 0;
    // draw each of the six sides
    makeSide(&index, glm::vec3(0, 0, 1), spacing, true, false);
    makeSide(&index, glm::vec3(0, 1, 0), spacing, false, false);
    makeSide(&index, glm::vec3(1, 0, 0), spacing, false, false);
    makeSide(&index, glm::vec3(0, -1, 0), spacing, false, false);
    makeSide(&index, glm::vec3(-1, 0, 0), spacing, false, false);
    makeSide(&index, glm::vec3(0, 0, -1), spacing, false, true);

}


void Cube::makeSide(int *index, glm::vec3 norm, double spacing, bool first, bool last)
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
    else { // (norm.z)
        tbi = 1;
        rli = 0;
        oi = 2;
        n = norm.z;
    }


    glm::vec3 v1, v2;

    // set corner point
    v1[rli] = -m_halfWidth;
    v1[tbi] = -m_halfWidth;
    v1[oi] = n * m_halfWidth;

    for (int i = 0; i < m_p1; i++) {

        v1[rli] = (i * spacing - m_halfWidth);
        v1[tbi] = (-m_halfWidth) * n;

        // double the first point if it isn't the start of the cube
        if (i != 0 || !first)
            addVertex(index, v1, norm);

        for (int j = 0; j <= m_p1; j++) {
            v1[tbi] = (j * spacing - m_halfWidth) * n;
            v2 = v1;
            v2[rli] = v1[rli] + spacing;

            addVertex(index, v1, norm);
            addVertex(index, v2, norm);
        }
        // double the last point if it isn't the end of the cube
        if (i != m_p1 - 1 || !last)
            addVertex(index, v2, norm);
    }
}


