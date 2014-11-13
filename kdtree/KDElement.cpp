#include "KDElement.h"

KDElement::KDElement(glm::vec4 min, glm::vec4 max, glm::vec4 pos, SceneElement *element)
{
    m_min = min;
    m_max = max;
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


KDLessThan::KDLessThan(int dimension)
    : m_dimension(dimension)
{
}


KDLessThan::~KDLessThan()
{
}


bool KDLessThan::operator()(const KDElement *left, const KDElement *right) const
{
    return left->m_pos[m_dimension] < right->m_pos[m_dimension];
}


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


