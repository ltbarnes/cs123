#ifndef RAYCUBE_H
#define RAYCUBE_H

#include "RayShape.h"

class RayCube : public RayShape
{
public:
    RayCube();
    ~RayCube();

    virtual glm::vec4 intersects(glm::vec4 p, glm::vec4 d);
    virtual glm::vec4 getNormal(glm::vec4 point);

};

#endif // RAYCUBE_H
