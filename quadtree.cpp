
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
#include <iostream>
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree()
{
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree &other)
{
    copy(other);
}
// quadtree assignment operator, given.
quadtree &quadtree::operator=(const quadtree &rhs)
{
    if (this != &rhs)
    {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG &imIn)
{
    stats s = stats(imIn);
    edge = (imIn.height() > imIn.width()) ? imIn.width() : imIn.height();
    edge = roundDownToPowerOf2(edge); // need it to be a power of two

    int d = log2(edge); // bc image should be square, thus this is the dimension
    root = buildTree(s, make_pair(0, 0), d);
}

quadtree::Node *quadtree::buildTree(stats &s, pair<int, int> ul, int dim)
{

    RGBAPixel currPixel = s.getAvg(ul, dim);
    double currVar = s.getVar(ul, dim);
    Node *currNode = new Node(ul, dim, currPixel, currVar);
    // base case
    if (dim == 0)
    {
        return currNode;
    }
    else
    {
        int x = ul.first;
        int y = ul.second;

        double sideLen = pow(2, (double)(dim - 1));

        currNode->NW = buildTree(s, make_pair(x, y), dim - 1);
        currNode->NE = buildTree(s, make_pair(x + sideLen, y), dim - 1);
        currNode->SW = buildTree(s, make_pair(x, y + sideLen), dim - 1);
        currNode->SE = buildTree(s, make_pair(x + sideLen, y + sideLen), dim - 1);
        return currNode;
    }
}

PNG quadtree::render() const
{
    PNG image = PNG(edge, edge);
    for (int x = 0; x < edge; x++)
    {
        for (int y = 0; y < edge; y++)
        {
            *image.getPixel(x, y) = root->avg;
        }
    }
    render_helper(root->NW, image);
    render_helper(root->NE, image);
    render_helper(root->SW, image);
    render_helper(root->SE, image);

    return image;
}

void quadtree::render_helper(Node *curr, PNG &im) const
{
    // Base Case
    if (curr == nullptr)
        return;
    else
    {
        int sideLen = pow(2, (double)curr->dim);
        int upLeftX = curr->upLeft.first;
        int upLeftY = curr->upLeft.second;

        for (int x = upLeftX; x < upLeftX + sideLen; x++)
        {
            for (int y = upLeftY; y < upLeftY + sideLen; y++)
            {
                *im.getPixel(x, y) = curr->avg;
            }
        }

        render_helper(curr->NW, im);
        render_helper(curr->NE, im);
        render_helper(curr->SW, im);
        render_helper(curr->SE, im);
    }
}

long quadtree::roundDownToPowerOf2(int n)
{
    long result = 1;
    while (result <= n)
    {
        result <<= 1; // equivalent to: result *= 2;
    }
    return result >> 1; // equivalent to: return result / 2;
}

int quadtree::pruneSizeHelper(Node *curr, const int tol) const
{
    if (curr == nullptr)
    {
        return 0;
    }
    else if (prunable(curr, tol) || isLeaf(curr))
    {
        return 1;
    }
    else
    {
        return pruneSizeHelper(curr->NW, tol) + pruneSizeHelper(curr->NE, tol) + pruneSizeHelper(curr->SW, tol) + pruneSizeHelper(curr->SE, tol);
    }
}

// REQUIRE, node != NULL
bool quadtree::isLeaf(Node *node) const
{
    return (node->NE == nullptr && node->NW == nullptr && node->SW == nullptr && node->SE == nullptr);
}

void quadtree::prune(const int tol)
{
    prune_helper(root, tol);
}

void quadtree::prune_helper(Node *curr, const int tol)
{
    // Base Case
    if (curr == nullptr)
        return;

    if (prunable(curr, tol))
    {
        clear_helper(curr->NW);
        clear_helper(curr->NE);
        clear_helper(curr->SW);
        clear_helper(curr->SE);
        curr->NW = nullptr;
        curr->NE = nullptr;
        curr->SW = nullptr;
        curr->SE = nullptr;
    }
    else
    {
        prune_helper(curr->NW, tol);
        prune_helper(curr->NE, tol);
        prune_helper(curr->SW, tol);
        prune_helper(curr->SE, tol);
    }
}

void quadtree::clear()
{
    clear_helper(root);
}

void quadtree::clear_helper(Node *currNode)
{
    // Base case
    if (currNode == nullptr)
        return;

    clear_helper(currNode->NW);
    clear_helper(currNode->NE);
    clear_helper(currNode->SW);
    clear_helper(currNode->SE);

    delete currNode;
}

// Implement Binary Search!!
// int quadtree::idealPrune(const int leaves) const
// {
//     int lower = 0;
//     int upper = INT32_MAX;

//     int currLeaves = 0;
//     int curr_tol = 0;
//     int best_tol = 0;

//     while (lower <= upper )
//     {
//         curr_tol = (lower + upper) / 2;
//         // cout << tol << " : "<< lower << "," << upper << endl;
//         if (pruneSize(curr_tol) > leaves)
//         {
//             best_tol = curr_tol;
//             upper = curr_tol;
//         }
//         else
//             lower = curr_tol;
//     }
//     if (lower == 0)
//     {
//         return 0;
//     }
//     else
//     {
//         return best_tol;
//     }
// }
int quadtree::idealPrune(const int leaves) const {

    int best_tol = 0, b = INT32_MAX, mid_tol = 0, a = 0;

    while(a + 1 < b) {
        mid_tol = (a + b) / 2;
        if (pruneSize(mid_tol) <= leaves) {
            b = mid_tol;            
            best_tol = mid_tol;
        } else { 
            a = mid_tol;
        }
    }

    return (a == 0) ? a : best_tol;

}

double quadtree::maxTol(Node *node, double mx) const
{
    // if (node == nullptr)
    //     return
    // mx = max(mx, node->var);
    // mx = max(mx, )
    return 0;
}

int quadtree::pruneSize(const int tol) const
{
    return pruneSizeHelper(root, tol);
}
void quadtree::copy(const quadtree &orig)
{
    Node *oldRoot = orig.root; // copying over the root
    edge = orig.edge;
    root = new Node(oldRoot->upLeft, oldRoot->dim, oldRoot->avg, oldRoot->var);
    root->NW = copy_helper(oldRoot->NW);
    root->NE = copy_helper(oldRoot->NE);
    root->SW = copy_helper(oldRoot->SW);
    root->SE = copy_helper(oldRoot->SE);
}

quadtree::Node *quadtree::copy_helper(Node *oldNode)
{
    if (oldNode == nullptr)
        return nullptr;

    Node *currNode = currNode = new Node(oldNode->upLeft, oldNode->dim, oldNode->avg, oldNode->var);
    currNode->NW = copy_helper(oldNode->NW);
    currNode->NE = copy_helper(oldNode->NE);
    currNode->SW = copy_helper(oldNode->SW);
    currNode->SE = copy_helper(oldNode->SE);

    return currNode;
}
