#ifndef AABB_H
#define AABB_H

#include "scenegraph/OpenGLScene.h"

class AABB
{
public:
    AABB();
    virtual ~AABB();

    // return true if the given ray intersects the box
    bool intersectsAABB(glm::vec4 p, glm::vec4 d);

    // sets the minimum and maximum points defining the box
    void setMin(glm::vec3 min);
    void setMax(glm::vec3 max);

    // returns the minimum and maximum point defining the box
    glm::vec3 getMin();
    glm::vec3 getMax();

    // returns the surface area of the box
    float getSA();

    // calculates the surface area of a box defined by the given min and max points
    static float calcSA(const glm::vec3 min, const glm::vec3 max);

private:
    // surface area
    float m_sa;

    // two corner points defining the cube
    glm::vec3 m_min, m_max;
};

#endif // AABB_H
