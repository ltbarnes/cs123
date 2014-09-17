#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape
{
public:
    Cylinder(int p1, int p2, float radius, float halfHeight);
    virtual ~Cylinder();

    virtual void calcVerts();

protected:
    virtual void make3DSlice(int *index, glm::vec2 first, glm::vec2 second);
    virtual void makeCircleSlice(int *index, glm::vec3 first, glm::vec3 second, glm::vec3 norm);
    virtual void makeRect(int *i, glm::vec3 tr, glm::vec3 tl, glm::vec3 bl, glm::vec3 br, glm::vec3 norm);

    float m_radius;
    float m_halfHeight;
};

#endif // CYLINDER_H
