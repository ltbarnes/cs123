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

    // return root node
    KDNode *getRoot();

    // return a list of all intersected objects
    QList<KDElement *> getIntersections(glm::vec4 p, glm::vec4 d);

private:
    // deletes subtree with root 'node'
    void deleteNode(KDNode *node);

    // returns a list of intersected objects if subtree with root 'node'
    QList<KDElement *> checkIntersections(glm::vec4 p, glm::vec4 d, KDNode *node);

    // recursively builds the kdtree
    void buildTree(int depth, QList<KDElement *> elements, KDNode *parent);

    // calculates the costs of spliting the plane at each of the objects borders
    // and sets the numLeft, numRight, leftMax, rightMin variables.
    void calcCosts(int dim, QList<KDElement *> mins, QList<KDElement *> maxs, KDNode *parent,
                   int *numRight, int *numLeft, glm::vec3 *leftMax, glm::vec3 *rightMin);

    // increments the left or right index variable and updates how many shapes
    // are on each side of a new splitting plane. The location of the plane is returned.
    float setLeft(int *mini, int *maxi, int *leftdiff, KDElement *mine, int dim, int *left);
    float setRight(int *mini, int *maxi, int *rightdiff, KDElement *maxe, int dim, int *left, int *right);

    // The root of the tree
    KDNode *m_root;
};

#endif // KDTREE_H
