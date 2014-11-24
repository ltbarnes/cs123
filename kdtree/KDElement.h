#ifndef KDELEMENT_H
#define KDELEMENT_H

#include "OpenGLScene.h"

class KDElement
{
public:
    friend class MinLessThan; // sorts based on min coordinates
    friend class MaxLessThan; // sorts based on max coordinates

    KDElement(glm::vec3 min, glm::vec3 max, glm::vec4 pos, SceneElement *element);
    virtual ~KDElement();

    // getters
    glm::vec4 getMin();
    glm::vec4 getMax();
    glm::vec4 getPos();
    CS123ScenePrimitive *getPrimitive();
    glm::mat4 getTrans();
    glm::mat4 getInv();

protected:
    // minimum, maximum, and center coordinates
    glm::vec4 m_min;
    glm::vec4 m_max;
    glm::vec4 m_pos;

    // shape primitive and trans matrix
    SceneElement *m_element;
};

/*
 * The following two classes are essentially the same but each one
 * overloads the operator in a slightly different way. They both
 * compare the corresponding coordinates based on a specified dimension.
 *
 * MinLessThan  -   compares minimum coordinates
 * MaxLessThan  -   compares maximum coordinates
 */
class MinLessThan
{
public:
    MinLessThan(int dimension);
    virtual ~MinLessThan();
    bool operator()(const KDElement *left, const KDElement *right) const;

private:
    int m_dimension;
};


class MaxLessThan
{
public:
    MaxLessThan(int dimension);
    virtual ~MaxLessThan();
    bool operator()(const KDElement *left, const KDElement *right) const;

private:
    int m_dimension;
};

#endif // KDELEMENT_H
