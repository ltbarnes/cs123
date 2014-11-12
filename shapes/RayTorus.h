#ifndef RAYTORUS_H
#define RAYTORUS_H

#include "RayShape.h"

class RayTorus : public RayShape
{
public:
    RayTorus();
    virtual ~RayTorus();

    virtual glm::vec4 intersects(glm::vec4 p, glm::vec4 d);

private:
    int findT(glm::vec4 p, glm::vec4 d, float *t1, float *t2);
};

#endif // RAYTORUS_H
