#ifndef RIPPLESPHERE_H
#define RIPPLESPHERE_H

#include "Shape.h"

class RippleSphere : public Shape
{
public:
    RippleSphere(int p1, int p2, float p3, float radius);
    virtual ~RippleSphere();

    virtual void calcVerts();
    virtual bool animate();

protected:
    virtual void make3Dslice(int *index, float thetaL, float thetaR, glm::vec3 top, glm::vec3 bottom);
    float map(float value, float origRange, float newRange);

    float m_radius;
    float m_depth;

    int m_rate;
    float m_ratef;

private:
    glm::vec2 rotate(glm::vec2 v, float angle);
    glm::vec3 calcNorm(float phi, float theta);

};

#endif // RIPPLESPHERE_H
