#include "KDNode.h"

KDNode::KDNode()
{
}


KDNode::~KDNode()
{
    // TODO: delete elements
}


void KDNode::setLeftChild(KDNode *left)
{
    m_left = left;
}


void KDNode::setRightChild(KDNode *right)
{
    m_right = right;
}


bool KDNode::hasLeftChild()
{
    return m_left != NULL;
}


bool KDNode::hasRightChild()
{
    return m_right != NULL;
}


KDNode *KDNode::getLeftChild()
{
    return m_left;
}


KDNode *KDNode::getRightChild()
{
    return m_right;
}


void KDNode::setElements(QList<KDElement *> elements)
{
    m_elements = elements;
}


QList<KDElement *> KDNode::getElements()
{
    return m_elements;
}


void KDNode::printElements()
{
    cout << "elements: ";
    int n = m_elements.size();
    for (int i = 0; i < n; i++)
    {
        cout << m_elements.at(i)->getPrimitive()->type << "->";
        cout << glm::to_string(m_elements.at(i)->getPos()) << "; ";
    }
    cout << endl;
}


