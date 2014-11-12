#ifndef RAYSHAPE_H
#define RAYSHAPE_H

#include "AABB.h"

class RayShape : public AABB
{
public:
    RayShape();
    virtual ~RayShape();

    /*
     * If the ray does not intersect an arbitrary vector is returned with
     * a 'w' value of infinity. If the ray does intersect then the 'xyz' value
     * of the returned vector is the surface normal at the intersection point
     * and the 'w' value is the distance from p to the intersection point.
     */
    virtual glm::vec4 intersects(glm::vec4 p, glm::vec4 d);

protected:
    // finds the minimum distance from p to an intersection point on the shape
    virtual int findT(glm::vec4 p, glm::vec4 d, float* t1, float* t2);
};

#endif // RAYSHAPE_H
