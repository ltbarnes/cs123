#include "RayShape.h"

RayShape::RayShape()
{
    setMin(glm::vec3(-0.5));
    setMax(glm::vec3(0.5));
}


RayShape::~RayShape()
{
}


void RayShape::getBounds(glm::vec3 *min, glm::vec3 *max, glm::mat4 trans)
{
    glm::vec4 blb, blf, brb, brf, tlb, tlf, trb, trf;

    blb = trans * glm::vec4(glm::vec3(-0.5f, -0.5f, -0.5f), 1);
    blf = trans * glm::vec4(glm::vec3(-0.5f, -0.5f,  0.5f), 1);
    brb = trans * glm::vec4(glm::vec3( 0.5f, -0.5f, -0.5f), 1);
    brf = trans * glm::vec4(glm::vec3( 0.5f, -0.5f,  0.5f), 1);
    tlb = trans * glm::vec4(glm::vec3(-0.5f,  0.5f, -0.5f), 1);
    tlf = trans * glm::vec4(glm::vec3(-0.5f,  0.5f,  0.5f), 1);
    trb = trans * glm::vec4(glm::vec3( 0.5f,  0.5f, -0.5f), 1);
    trf = trans * glm::vec4(glm::vec3( 0.5f,  0.5f,  0.5f), 1);

    // in case there are rotations
    *min = glm::vec3(glm::min(blb, glm::min(blf, glm::min(brb, glm::min(brf, glm::min(tlb, glm::min(tlf, glm::min(trb, trf))))))));
    *max = glm::vec3(glm::max(blb, glm::max(blf, glm::max(brb, glm::max(brf, glm::max(tlb, glm::max(tlf, glm::max(trb, trf))))))));
}

