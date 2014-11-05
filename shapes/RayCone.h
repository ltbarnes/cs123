#ifndef RAYCONE_H
#define RAYCONE_H

#include "RayShape.h"

class RayCone : public RayShape
{
public:
    RayCone();
    virtual ~RayCone();

    virtual float intersects(glm::vec4 p, glm::vec4 d);
    virtual glm::vec4 getNormal(glm::vec4 point);

protected:
    virtual int findT(glm::vec4 p, glm::vec4 d, float* t1, float* t2);
};

#endif // RAYCONE_H
