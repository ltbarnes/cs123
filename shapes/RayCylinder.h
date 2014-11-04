#ifndef RAYCYLINDER_H
#define RAYCYLINDER_H

#include "RayShape.h"

class RayCylinder : public RayShape
{
public:
    RayCylinder();
    virtual ~RayCylinder();

    virtual float intersects(glm::vec4 p, glm::vec4 d);

private:
    int findT(glm::vec4 p, glm::vec4 d, float *t1, float *t2);
};

#endif // RAYCYLINDER_H
