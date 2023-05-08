#include "qtvar.h"

bool qtvar::prunable(Node* node, const int tol) const {
    // if the nodes variance is less than tolerance
    return (node->var < tol);
}
