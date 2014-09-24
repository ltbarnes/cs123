#ifndef RIPPLE_H
#define RIPPLE_H

#include "Shape.h"

class Ripple : public Shape
{
public:
    Ripple(int p1, int p2, float width);
    virtual ~Ripple();

    virtual void calcVerts();
    virtual bool animate();
    virtual void render();

protected:
    float m_radius;

private:
    glm::vec3 calcNormal(glm::vec3 p);
    float f(float x, float y);
    float df(float x, float y);

    int m_rate;

};

#endif // RIPPLE_H
