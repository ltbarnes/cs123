#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape
{
public:
    Cube(int p1, int p2, float width, NormalRenderer *normRenderer);
    virtual ~Cube();

    virtual void calcVerts();
    virtual void render();

protected:
    virtual void makeSide(glm::vec3 norm);
    virtual void makeSquare(glm::vec3 tr, glm::vec3 tl, glm::vec3 bl, glm::vec3 br, glm::vec3 norm);
};

#endif // CUBE_H
