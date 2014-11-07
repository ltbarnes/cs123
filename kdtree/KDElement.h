#ifndef KDELEMENT_H
#define KDELEMENT_H

#include "OpenGLScene.h"

class KDElement
{
public:
    friend class KDLessThan;
    KDElement(glm::vec4 min, glm::vec4 max, glm::vec4 pos, SceneElement *element);
    virtual ~KDElement();

    glm::vec4 getMin();
    glm::vec4 getMax();
    glm::vec4 getPos();
    CS123ScenePrimitive *getPrimitive();
    glm::mat4 getTrans();

protected:
    glm::vec4 m_min;
    glm::vec4 m_max;
    glm::vec4 m_pos;
    SceneElement *m_element;
};


class KDLessThan
{
public:
    KDLessThan(int dimension);
    virtual ~KDLessThan();
    bool operator()(const KDElement *left, const KDElement *right) const;

private:
    int m_dimension;
};

#endif // KDELEMENT_H
