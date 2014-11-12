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

    void setLeftChild(KDNode *left);
    void setRightChild(KDNode *right);
    bool hasLeftChild();
    bool hasRightChild();
    KDNode *getLeftChild();
    KDNode *getRightChild();

    void setElements(QList<KDElement *> elements);
    QList<KDElement *> getElements();

    void printElements();

private:
    KDNode *m_left;
    KDNode *m_right;
    int m_depth;
    QList<KDElement *> m_elements;
};

#endif // KDNODE_H
