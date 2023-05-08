#ifndef _QTCOUNT_H_
#define _QTCOUNT_H_

#include <cmath>
#include <utility>

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "quadtree.h"

using namespace std;
using namespace cs221util;

class qtcount : public quadtree {
public:
    qtcount(PNG& im) : quadtree(im) {}

private:
/**
 * Node pruned if all leaves in its subtree have colour-distance within tolerance of its average. 
 * Distances between colours are computed as the sum, over each colour channel, of the pixel value differences, squared. 
 * For reference, to create the pruned totem pole above, we used a tolerance of 4000
*/
    bool prunable(Node* node, const int tol) const;

    bool compareLeafTol(Node* node, Node* curr, const int tol) const;

    double calculateSqTol(Node* node, Node* curr, const int tol) const;
};

#endif
