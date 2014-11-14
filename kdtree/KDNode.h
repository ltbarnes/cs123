#ifndef KDNODE_H
#define KDNODE_H

#include <QList>
#include "KDTree.h"
#include "shapes/AABB.h"

class KDNode : public AABB
{
public:
    KDNode();
    virtual ~KDNode();

    // setters
    void setLeftChild(KDNode *left);
    void setRightChild(KDNode *right);
    void setElements(QList<KDElement *> elements);

    // getters
    KDNode *getLeftChild();
    KDNode *getRightChild();
    QList<KDElement *> getElements();

    // return true if node has appropriate child
    bool hasLeftChild();
    bool hasRightChild();

    // prints the tree (breadth first)
    void printElements();

private:
    // children nodes
    KDNode *m_left;
    KDNode *m_right;

    // depth of node
    int m_depth;

    // list of elements (empty if not a leaf node)
    QList<KDElement *> m_elements;
};

#endif // KDNODE_H
