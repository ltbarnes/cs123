#include "KDElement.h"

KDElement::KDElement(glm::vec3 min, glm::vec3 max, glm::vec4 pos, SceneElement *element)
{
    m_min = glm::vec4(min, 1.f);
    m_max = glm::vec4(max, 1.f);
    m_pos = pos;
    m_element = element;
}


KDElement::~KDElement()
{
    delete m_element->primitive->material.bumpMap;
    delete m_element->primitive->material.textureMap;
    delete m_element->primitive;
    delete m_element;
}


glm::vec4 KDElement::getMin()
{
    return m_min;
}


glm::vec4 KDElement::getMax()
{
    return m_max;
}


glm::vec4 KDElement::getPos()
{
    return m_pos;
}


CS123ScenePrimitive *KDElement::getPrimitive()
{
    return m_element->primitive;
}


glm::mat4 KDElement::getTrans()
{
    return m_element->trans;
}


// ////////////////MinLessThan////////////////// //

MinLessThan::MinLessThan(int dimension)
    : m_dimension(dimension)
{
}


MinLessThan::~MinLessThan()
{
}


bool MinLessThan::operator()(const KDElement *left, const KDElement *right) const
{
    return left->m_min[m_dimension] < right->m_min[m_dimension];
}



// ////////////////MaxLessThan////////////////// //

MaxLessThan::MaxLessThan(int dimension)
    : m_dimension(dimension)
{
}


MaxLessThan::~MaxLessThan()
{
}


bool MaxLessThan::operator()(const KDElement *left, const KDElement *right) const
{
    return left->m_max[m_dimension] < right->m_max[m_dimension];
}


