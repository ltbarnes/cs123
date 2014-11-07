#ifndef AABB_H
#define AABB_H

#include "scenegraph/OpenGLScene.h"

class AABB
{
public:
    AABB();
    virtual ~AABB();

    bool intersectsAABB(glm::vec4 p, glm::vec4 d);
    void setMin(glm::vec3 min);
    void setMax(glm::vec3 max);
    glm::vec3 getMin();
    glm::vec3 getMax();
    float getSA();

private:
    void calcSA();

    float m_sa;
    bool m_minSet, m_maxSet;

    // two corner points defining the cube
    glm::vec3 m_min, m_max;
};

#endif // AABB_H
