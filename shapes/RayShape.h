#ifndef RAYSHAPE_H
#define RAYSHAPE_H

#include "OpenGLScene.h"

class RayShape
{
public:
    RayShape();
    virtual ~RayShape();

    virtual float intersects(glm::vec4 p, glm::vec4 d);
    virtual glm::vec4 getNormal(glm::vec4 point);

protected:
    virtual int findT(glm::vec4 p, glm::vec4 d, float* t1, float* t2);
    bool intersectsPlane(glm::vec3 point, glm::vec3 norm, glm::vec3 p, glm::vec3 d);
};

#endif // RAYSHAPE_H