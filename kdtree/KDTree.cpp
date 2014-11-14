#include <QList>
#include <QSet>
#include "KDTree.h"
#include "KDNode.h"

KDTree::KDTree(QList<KDElement *> elements, glm::vec3 min, glm::vec3 max)
{
    m_root = new KDNode();
    m_root->setMin(min);
    m_root->setMax(max);
    buildTree(0, elements, m_root);
}


KDTree::~KDTree()
{
    deleteNode(m_root);
}


void KDTree::deleteNode(KDNode *node) {
    if (node->hasLeftChild())
        deleteNode(node->getLeftChild());
    if (node->hasRightChild())
        deleteNode(node->getRightChild());
    delete node;
}


KDNode *KDTree::getRoot()
{
    return m_root;
}


QList<KDElement *> KDTree::getIntersections(glm::vec4 p, glm::vec4 d)
{
    return checkIntersections(p, d, m_root);
}


QList<KDElement *> KDTree::checkIntersections(glm::vec4 p, glm::vec4 d, KDNode *node)
{
    QList<KDElement *> elements = node->getElements();
    if (node->intersectsAABB(p, d)) {
        if (node->hasLeftChild())
            elements.append(checkIntersections(p, d, node->getLeftChild()));
        if (node->hasRightChild())
            elements.append(checkIntersections(p, d, node->getRightChild()));
    }
    return elements;

}


void KDTree::buildTree(int depth, QList<KDElement *> elements, KDNode *parent)
{
    if (elements.size() <= 1 || depth >= 24) {
        parent->setElements(elements);
        parent->setLeftChild(NULL);
        parent->setRightChild(NULL);
        return;
    }

    int dim = depth % 3;
    assert(dim >= 0 && dim < 3);

    int n = elements.size();
    glm::vec3 leftMax, rightMin;

    // number of shapes in each child
    int numLeft = 0;
    int numRight = n;

    qSort(elements.begin(), elements.end(), PosLessThan(dim));

    QList<KDElement *> mins = elements;
    QList<KDElement *> maxs = elements;

    qSort(mins.begin(), mins.end(), MinLessThan(dim));
    qSort(maxs.begin(), maxs.end(), MaxLessThan(dim));

    calcCosts(dim, mins, maxs, parent, &numRight, &numLeft, &leftMax, &rightMin);

    KDNode *left = NULL;
    KDNode *right = NULL;

    if (numLeft) {
        left = new KDNode();
        left->setMin(parent->getMin());
        left->setMax(leftMax);
        buildTree(depth + 1, mins.mid(0, numLeft), left);
    }
    if (numRight) {
        right = new KDNode();
        right->setMin(rightMin);
        right->setMax(parent->getMax());
        buildTree(depth + 1, maxs.mid((n - numRight), numRight), right);
    }
    parent->setLeftChild(left);
    parent->setRightChild(right);
}


void KDTree::calcCosts(int dim, QList<KDElement *> mins, QList<KDElement *> maxs, KDNode *parent,
                       int *numRight, int *numLeft, glm::vec3 *leftMax, glm::vec3 *rightMin)
{

    float saParent = parent->getSA();
    float cost, saLeft, saRight;
    float bestCost = std::numeric_limits<float>::infinity();
    glm::vec3 min, max;
    KDElement *mine, *maxe;
    float val;

    int n = mins.size();
    int mini = 0;
    int maxi = 0;
    int left = 0;
    int right = n;
    int leftdiff = 1;
    int rightdiff = 0;

    while (mini < n) {

        mine = mins.at(mini);
        maxe = maxs.at(maxi);

        if (mine->getMin()[dim] < maxe->getMax()[dim])
            val = setLeft(&mini, &maxi, &leftdiff, mine, dim, &left);
        else
            val = setRight(&mini, &maxi, &rightdiff, maxe, dim, &left, &right);

        min = parent->getMin();
        max = parent->getMax();
        min[dim] = val;
        max[dim] = val;

        // cost
        saLeft = KDNode::calcSA(parent->getMin(), max);
        saRight = KDNode::calcSA(min, parent->getMax());
        cost = (saLeft / saParent) * left + (saRight / saParent) * right;

        // check cost against best cost
        if (cost < bestCost) {
            bestCost = cost;
            *numRight = right;
            *numLeft = left;
            *leftMax = max;
            *rightMin = min;
        }
    }

    while (maxi < n) {

        maxe = maxs.at(maxi);
        val = setRight(&mini, &maxi, &rightdiff, maxe, dim, &left, &right);

        min = parent->getMin();
        max = parent->getMax();
        min[dim] = val;
        max[dim] = val;

        // cost
        saLeft = KDNode::calcSA(parent->getMin(), max);
        saRight = KDNode::calcSA(min, parent->getMax());
        cost = (saLeft / saParent) * left + (saRight / saParent) * right;

        // check cost against best cost
        if (cost < bestCost) {
            bestCost = cost;
            *numRight = right;
            *numLeft = left;
            *leftMax = max;
            *rightMin = min;
        }
    }
}

float KDTree::setLeft(int *mini, int *maxi, int *leftdiff, KDElement *mine, int dim, int* left)
{
    (*mini)++;
    int temp = *mini - *maxi;
    if (temp > *leftdiff)
        (*left)++;
    *leftdiff = temp;
    return mine->getMin()[dim];
}


float KDTree::setRight(int *mini, int *maxi, int *rightdiff, KDElement *maxe, int dim, int* left, int* right)
{
    (*maxi)++;
    int temp = *mini - *maxi;
    if (temp >= *rightdiff)
        (*left)++;
    (*right)--;
    *rightdiff = temp;
    return maxe->getMax()[dim];
}


// ////// Potentially useful debugging methods ////// //

//QList<KDElement *> KDTree::getElements(KDNode *node)
//{
//    QList<KDElement *> elements = node->getElements();
//    if (node->hasLeftChild())
//        elements.append(getElements(node->getLeftChild()));
//    if (node->hasRightChild())
//        elements.append(getElements(node->getRightChild()));
//    return elements;
//}


//void KDTree::printTree(KDNode *node) {
//    QList<KDNode *> q;
//    q.append(node);
//    while(!q.isEmpty()) {
//        node = q.at(0);
//        q.removeAt(0);

//        if (node != NULL) {
//            KDTree::printNode(node);
//            if (node->hasLeftChild())
//                q.append(node->getLeftChild());
//            if (node->hasRightChild())
//                q.append(node->getRightChild());
//        }
//    }
//}


//void KDTree::printNode(KDNode *node)
//{
//    cout << "NODE: " << node << endl;
//    cout << "min: " << glm::to_string(node->getMin()) << endl;
//    cout << "max: " << glm::to_string(node->getMax()) << endl;
//    cout << "left: " << node->getLeftChild() << endl;
//    cout << "right: " << node->getRightChild() << endl;
//    node->printElements();
//}


