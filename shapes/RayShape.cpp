#include "RayShape.h"

RayShape::RayShape()
{
}


RayShape::~RayShape()
{
}


float RayShape::intersects(glm::vec4 p, glm::vec4 d)
{
    return 0.f;
}


int RayShape::findT(glm::vec4 p, glm::vec4 d, float *t1, float *t2)
{
    return 0;
}


bool RayShape::intersectsPlane(glm::vec3 point, glm::vec3 norm, glm::vec3 p, glm::vec3 d)
{
    return false;
}

