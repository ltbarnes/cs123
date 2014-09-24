#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape
{
public:
    Cylinder(int p1, int p2, float radius, float halfHeight);
    virtual ~Cylinder();

    virtual void calcVerts();
    virtual void render();

protected:
    virtual void make3DSlice(int *index, glm::vec2 left, glm::vec2 right);
    virtual void makeTopSlice(int *index, glm::vec3 left, glm::vec3 right);
    virtual void makeBottomSlice(int *index, glm::vec3 left, glm::vec3 right);

    float m_radius;
    float m_halfHeight;
};

#endif // CYLINDER_H
