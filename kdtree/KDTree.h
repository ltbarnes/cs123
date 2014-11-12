#ifndef KDTREE_H
#define KDTREE_H

#include "OpenGLScene.h"
#include "KDElement.h"
class KDNode;

class KDTree
{
public:
    KDTree(QList<KDElement *> elements, glm::vec3 min, glm::vec3 max);
    virtual ~KDTree();

    KDNode *getRoot();

    static void printTree(KDNode *node);
    static void printNode(KDNode *node);

    QList<KDElement *> getIntersections(glm::vec4 p, glm::vec4 d);

private:
    void deleteNode(KDNode *node);
    QList<KDElement *> checkIntersections(glm::vec4 p, glm::vec4 d, KDNode *node);

    void buildTree(int depth, QList<KDElement *> elements, KDNode *parent);

    void calcCosts(int dim, QList<KDElement *> mins, QList<KDElement *> maxs, KDNode *parent,
                   int *rightI, int *leftI, glm::vec3 *leftMax, glm::vec3 *rightMin);
    float setLeft(int *mini, int *maxi, int *leftdiff, KDElement *mine, int dim, int *left);
    float setRight(int *mini, int *maxi, int *rightdiff, KDElement *maxe, int dim, int *left, int *right);
    KDNode *m_root;
};

#endif // KDTREE_H
