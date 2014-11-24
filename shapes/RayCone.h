#ifndef RAYCONE_H
#define RAYCONE_H

#include "RayShape.h"

class RayCone : public RayShape
{
public:
    RayCone();
    virtual ~RayCone();

    virtual glm::vec4 intersects(glm::vec4 p, glm::vec4 d);
    virtual glm::vec2 getUV(glm::vec4 point, glm::vec4 n);

protected:
    virtual int findT(glm::vec4 p, glm::vec4 d, float* t1, float* t2);
};

#endif // RAYCONE_H
