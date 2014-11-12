#include "RayTorus.h"

RayTorus::RayTorus()
{
}


RayTorus::~RayTorus()
{
}


glm::vec4 RayTorus::intersects(glm::vec4 p, glm::vec4 d)
{
    return glm::vec4(0, 0, 0, std::numeric_limits<float>::infinity());
}


int RayTorus::findT(glm::vec4 p, glm::vec4 d, float *t1, float *t2)
{
    return 0;
}


