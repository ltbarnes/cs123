#ifndef RAYSPHERE_H
#define RAYSPHERE_H

#include "RayShape.h"

class RaySphere : public RayShape
{
public:
    RaySphere();
    virtual ~RaySphere();

    virtual glm::vec4 intersects(glm::vec4 p, glm::vec4 d);
    virtual glm::vec2 getUV(glm::vec4 point, glm::vec4 n);

private:
    virtual int findT(glm::vec4 p, glm::vec4 d, float *t1, float *t2);

};

#endif // RAYSPHERE_H
