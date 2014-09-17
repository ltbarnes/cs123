#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape
{
public:
    Cube(int p1, float halfWidth);
    virtual ~Cube();

    virtual void calcVerts();

protected:
    virtual void makeSide(int *index, glm::vec3 norm);

    float m_halfWidth;
};

#endif // CUBE_H
