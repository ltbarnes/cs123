#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere(int p1, int p2, float radius);
    virtual ~Sphere();

    virtual void calcVerts();

protected:
    virtual void make3Dslice(int *index, float thetaR, float thetaL);
    virtual void calcSliceSeg(int *index, float thetaR, float thetaL, float phi);

    float m_radius;
};

#endif // SPHERE_H
