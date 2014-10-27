#ifndef CONE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape
{
public:
    Cone(int p1, int p2, float radius, float halfHeight);
    virtual ~Cone();

    virtual void calcVerts();

protected:
    virtual void make3DSlice(int *index, glm::vec2 left, glm::vec2 right, float normSlope, float leftU, float rightU);
    void makeBottomSlice(int *index, glm::vec3 left, glm::vec3 right);

    float m_radius;
    float m_halfHeight;
};

#endif // CONE_H
