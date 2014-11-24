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
    virtual glm::vec4 intersects(glm::vec4 p, glm::vec4 d) = 0;

//    virtual glm::vec2 getUV(glm::vec4 point, glm::vec4 n) = 0;
    virtual glm::vec2 getUV(glm::vec4 point, glm::vec4 n);

    void getBounds(glm::vec3 *min, glm::vec3 *max, glm::mat4 trans);
};

#endif // RAYSHAPE_H
