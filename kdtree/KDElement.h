#ifndef KDELEMENT_H
#define KDELEMENT_H

#include "OpenGLScene.h"

class KDElement
{
public:
    friend class PosLessThan; // sorts based on position
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

protected:
    // minimum, maximum, and center coordinates
    glm::vec4 m_min;
    glm::vec4 m_max;
    glm::vec4 m_pos;

    // shape primitive and trans matrix
    SceneElement *m_element;
};

/*
 * The following 3 classes are essentially the same but each one
 * overloads the operator in a slightly different way. They all
 * compare the corresponding coordinates based on a specified dimension.
 *
 * PosLessThan  -   compares center position
 * MinLessThan  -   compares minimum coordinates
 * MaxLessThan  -   compares maximum coordinates
 */
class PosLessThan
{
public:
    PosLessThan(int dimension);
    virtual ~PosLessThan();
    bool operator()(const KDElement *left, const KDElement *right) const;

private:
    int m_dimension;
};


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
