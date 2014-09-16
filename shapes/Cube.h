#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape
{
public:
    Cube(int p1, int p2, float width, NormalRenderer *normRenderer);
    virtual ~Cube();

    virtual void update();
    virtual void render();
};

#endif // CUBE_H
