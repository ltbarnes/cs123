#include "Cube.h"

Cube::Cube(int p1, int p2, float width, NormalRenderer *normRenderer)
    : Shape()
{
    m_p1 = p1;
    m_p2 = p2;
    m_radius = width;

}

Cube::~Cube()
{

}


void Cube::calcVerts()
{

}


void Cube::makeSide(glm::vec3 norm)
{

}


void Cube::makeSquare(glm::vec3 tr, glm::vec3 tl, glm::vec3 bl, glm::vec3 br, glm::vec3 norm)
{

}


void Cube::render()
{

}
