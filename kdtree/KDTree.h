#ifndef KDTREE_H
#define KDTREE_H

#include "OpenGLScene.h"
#include "KDElement.h"
class KDNode;

class KDTree
{
public:
    KDTree(QList<KDElement *> elements);
    virtual ~KDTree();

private:
    KDNode *m_root;
};

#endif // KDTREE_H
