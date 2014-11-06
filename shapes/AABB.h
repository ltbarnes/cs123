#ifndef AABB_H
#define AABB_H

#include "OpenGLScene.h"

class AABB
{
public:
    AABB();
    virtual ~AABB();

    bool intersects(glm::vec4 p, glm::vec4 d);
    void setMin(glm::vec3 min);
    void setMax(glm::vec3 max);
    float getSA();

private:
    void calcSA();

    float m_sa;
    bool m_minSet, m_maxSet;

    // two corner points defining the cube
    glm::vec3 m_max, m_min;
};

#endif // AABB_H
