#include "RayShape.h"

RayShape::RayShape()
{
    setMin(glm::vec3(-0.5));
    setMax(glm::vec3(0.5));
}


RayShape::~RayShape()
{
}


glm::vec4 RayShape::intersects(glm::vec4 p, glm::vec4 d)
{
    return glm::vec4(0, 0, 0, std::numeric_limits<float>::infinity());
}


int RayShape::findT(glm::vec4 p, glm::vec4 d, float *t1, float *t2)
{
    return 0;
}

