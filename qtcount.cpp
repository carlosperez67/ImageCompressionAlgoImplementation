#include "qtcount.h"

bool qtcount::prunable(Node *node, const int tol) const
{

    bool b1 = compareLeafTol(node, node->NW, tol);
    bool b2 = compareLeafTol(node, node->NE, tol);
    bool b3 = compareLeafTol(node, node->SW, tol);
    bool b4 = compareLeafTol(node, node->SE, tol);
    return (b1 && b2 && b3 && b4);
}

bool qtcount::compareLeafTol(Node *node, Node *curr, const int tol) const
{
    if (curr == nullptr) 
    {
        return true;
    }
    if (curr->dim == 0)
    {
        return (calculateSqTol(node, curr, tol) <= tol);
    }
    else
    {

        // if (node->NE != nullptr)
        bool result = compareLeafTol(node, curr->NE, tol);
        // if (node->NW != nullptr)
        result = result && compareLeafTol(node, curr->NW, tol);
        // if (node->SE != nullptr)
        result = result && compareLeafTol(node, curr->SE, tol);
        // if (node->SW != nullptr)
        result = result && compareLeafTol(node, curr->SW, tol);
        return result;
    }
}

double qtcount::calculateSqTol(Node *node, Node *curr, const int tol) const
{
    double redSqDiff = pow(node->avg.r - curr->avg.r,2);
    double greenSqDiff = pow(node->avg.g - curr->avg.g, (double)2);
    double blueSqDiff = pow((double)node->avg.b - (double)curr->avg.b, (double)2);

    return redSqDiff + greenSqDiff + blueSqDiff;
}