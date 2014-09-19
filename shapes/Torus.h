#ifndef TORUS_H
#define TORUS_H

#include "Shape.h"

class Torus : public Shape
{
public:
    Torus(int p1, int p2, float p3, float radius);
    virtual ~Torus();

    virtual void calcVerts();

protected:
    virtual void make3Dslice(int *index, float phiL, float phiR);

    float m_radius;
    float m_r;
    float m_R;
};

#endif // TORUS_H
