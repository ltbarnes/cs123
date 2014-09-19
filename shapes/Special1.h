#ifndef SPECIAL1_H
#define SPECIAL1_H

#include "Shape.h"

class Special1 : public Shape
{
public:
    Special1(int p1, int p2, float p3, float radius);
    virtual ~Special1();

    virtual void calcVerts();

protected:
    virtual void make3Dslice(int *index, float phiL, float phiR);

    float m_radius;
};

#endif // SPECIAL1_H
