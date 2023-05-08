#ifndef _QTVAR_H_
#define _QTVAR_H_

#include <cmath>
#include <utility>

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "quadtree.h"

using namespace std;
using namespace cs221util;

class qtvar : public quadtree {
public:
    qtvar(PNG& im) : quadtree(im) {}

private:
/**
 * Returns true if the node's var is less than the given tolerance
*/
    bool prunable(Node* node, const int tol) const;
};

#endif
