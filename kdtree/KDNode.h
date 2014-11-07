#ifndef KDNODE_H
#define KDNODE_H

#include <QList>
#include "KDTree.h"
#include "shapes/AABB.h"

class KDNode : private AABB
{
public:
    KDNode();
    virtual ~KDNode();

private:
    KDNode *left;
    KDNode *right;
    int m_depth;
    QList<KDElement *> m_elements;
};

#endif // KDNODE_H
