#include "stats.h"

stats::stats(PNG &im)
{
    sumRed.resize(im.width(), vector<long>(im.height()));
    sumGreen.resize(im.width(), vector<long>(im.height()));
    sumBlue.resize(im.width(), vector<long>(im.height()));
    sumsqRed.resize(im.width(), vector<long>(im.height()));
    sumsqGreen.resize(im.width(), vector<long>(im.height()));
    sumsqBlue.resize(im.width(), vector<long>(im.height()));

    for (unsigned int x = 0; x < im.width(); x++)
    {
        for (unsigned int y = 0; y < im.height(); y++)
        {
            {
                RGBAPixel *pixel = im.getPixel(x, y);

                int rV = pixel->r;
                int gV = pixel->g;
                int bV = pixel->b;
                long rV2 = pow(rV, 2);
                long gV2 = pow(gV, 2);
                long bV2 = pow(bV, 2);

                if (x == 0 && y == 0)
                { // initializing the first entry in the matrix
                    sumRed[x][y] = rV;
                    sumsqRed[x][y] = rV2;
                    sumGreen[x][y] = gV;
                    sumsqGreen[x][y] = gV2;
                    sumBlue[x][y] = bV;
                    sumsqBlue[x][y] = bV2;
                }
                else if (x == 0)
                { // case where its on the x axis
                    sumRed[x][y] = sumRed[x][y - 1] + rV;
                    sumsqRed[x][y] = sumsqRed[x][y - 1] + rV2;

                    sumGreen[x][y] = sumGreen[x][y - 1] + gV;
                    sumsqGreen[x][y] = sumsqGreen[x][y - 1] + gV2;

                    sumBlue[x][y] = sumBlue[x][y - 1] + bV;
                    sumsqBlue[x][y] = sumsqBlue[x][y - 1] + bV2;
                }
                else if (y == 0)
                { // case where its on the y axis
                    sumRed[x][y] = sumRed[x - 1][y] + rV;
                    sumsqRed[x][y] = sumsqRed[x - 1][y] + rV2;

                    sumGreen[x][y] = sumGreen[x - 1][y] + gV;
                    sumsqGreen[x][y] = sumsqGreen[x - 1][y] + gV2;

                    sumBlue[x][y] = sumBlue[x - 1][y] + bV;
                    sumsqBlue[x][y] = sumsqBlue[x - 1][y] + bV2;
                }
                else
                { // case where its in the interior matrix
                    sumRed[x][y] = sumRed[x - 1][y] + sumRed[x][y - 1] - sumRed[x - 1][y - 1] + rV;
                    sumsqRed[x][y] = sumsqRed[x - 1][y] + sumsqRed[x][y - 1] - sumsqRed[x - 1][y - 1] + rV2;

                    sumGreen[x][y] = sumGreen[x - 1][y] + sumGreen[x][y - 1] - sumGreen[x - 1][y - 1] + gV;
                    sumsqGreen[x][y] = sumsqGreen[x - 1][y] + sumsqGreen[x][y - 1] - sumsqGreen[x - 1][y - 1] + gV2;

                    sumBlue[x][y] = sumBlue[x - 1][y] + sumBlue[x][y - 1] - sumBlue[x - 1][y - 1] + bV;
                    sumsqBlue[x][y] = sumsqBlue[x - 1][y] + sumsqBlue[x][y - 1] - sumsqBlue[x - 1][y - 1] + bV2;
                }
            }
        }
    }
}

long stats::getSum(char channel, pair<int, int> ul, int dim)
{
    int k = pow(2, dim);
    int x = ul.first;
    int y = ul.second;

    if (x == 0 && y == 0)
    {
        switch (channel)
        {
        case 'r':
            return sumRed[x + k - 1][y + k - 1];
        case 'g':
            return sumGreen[x + k - 1][y + k - 1];
        case 'b':
            return sumBlue[x + k - 1][y + k - 1];
        default:
            return -1;
        }
    }
    else if (x == 0)
    {
        switch (channel)
        {
        case 'r':
            return sumRed[x + k - 1][y + k - 1] - sumRed[x + k - 1][y - 1];
        case 'g':
            return sumGreen[x + k - 1][y + k - 1] - sumGreen[x + k - 1][y - 1];
        case 'b':
            return sumBlue[x + k - 1][y + k - 1] - sumBlue[x + k - 1][y - 1];
        default:
            return -1;
        }
    }
    else if (y == 0)
    {
        switch (channel)
        {
        case 'r':
            return sumRed[x + k - 1][y + k - 1] - sumRed[x - 1][y + k - 1];
        case 'g':
            return sumGreen[x + k - 1][y + k - 1] - sumGreen[x - 1][y + k - 1];
        case 'b':
            return sumBlue[x + k - 1][y + k - 1] - sumBlue[x - 1][y + k - 1];
        default:
            return -1;
        }
    }
    else
    {
        switch (channel)
        {
        case 'r':
            return sumRed[x+k-1][y+k-1] - sumRed[x-1][y+k-1]
            - sumRed[x+k-1][y-1]+ sumRed[x-1][y-1];
        case 'g':
            return sumGreen[x+k-1][y+k-1] - sumGreen[x-1][y+k-1]
            - sumGreen[x+k-1][y-1]+ sumGreen[x-1][y-1];
        case 'b':
            return sumBlue[x + k - 1][y + k - 1] - sumBlue[x - 1][y + k - 1] - sumBlue[x + k - 1][y - 1] + sumBlue[x - 1][y - 1];
        default:
            return -1;
        }
    }
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim)
{
    int k = pow(2, dim);
    int x = ul.first;
    int y = ul.second;

    if (x == 0 && y == 0)
    {
        switch (channel)
        {
        case 'r':
            return sumsqRed[x + k - 1][y + k - 1];
        case 'g':
            return sumsqGreen[x + k - 1][y + k - 1];
        case 'b':
            return sumsqBlue[x + k - 1][y + k - 1];
        default:
            return -1;
        }
    }
    else if (x == 0)
    {
        switch (channel)
        {
        case 'r':
            return sumsqRed[x + k - 1][y + k - 1] - sumsqRed[x + k - 1][y - 1];
        case 'g':
            return sumsqGreen[x + k - 1][y + k - 1] - sumsqGreen[x + k - 1][y - 1];
        case 'b':
            return sumsqBlue[x + k - 1][y + k - 1] - sumsqBlue[x + k - 1][y - 1];
        default:
            return -1;
        }
    }
    else if (y == 0)
    {
        switch (channel)
        {
        case 'r':
            return sumsqRed[x + k - 1][y + k - 1] - sumsqRed[x - 1][y + k - 1];
        case 'g':
            return sumsqGreen[x + k - 1][y + k - 1] - sumsqGreen[x - 1][y + k - 1];
        case 'b':
            return sumsqBlue[x + k - 1][y + k - 1] - sumsqBlue[x - 1][y + k - 1];
        default:
            return -1;
        }
    }
    else
    {
        switch (channel)
        {
        case 'r':
            return sumsqRed[x + k - 1][y + k - 1] - sumsqRed[x - 1][y + k - 1] - sumsqRed[x + k - 1][y - 1] + sumsqRed[x - 1][y - 1];
        case 'g':
            return sumsqGreen[x + k - 1][y + k - 1] - sumsqGreen[x - 1][y + k - 1] - sumsqGreen[x + k - 1][y - 1] + sumsqGreen[x - 1][y - 1];
        case 'b':
            return sumsqBlue[x + k - 1][y + k - 1] - sumsqBlue[x - 1][y + k - 1] - sumsqBlue[x + k - 1][y - 1] + sumsqBlue[x - 1][y - 1];
        default:
            return -1;
        }
    }
}

long stats::rectArea(int dim)
{
    return pow(2, dim) * pow(2, dim);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim)
{
    double sumSq = getSumSq('r', ul, dim) + getSumSq('g', ul, dim) + getSumSq('b', ul, dim);
    double sqOfSum = pow(getSum('r', ul, dim), 2) + pow(getSum('g', ul, dim), 2) + pow(getSum('b', ul, dim), 2);

    return sumSq - (sqOfSum / (double) rectArea(dim));
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim)
{
    long area = rectArea(dim);

    // implicit cast to int
    int red = (double) getSum('r', ul, dim) / (double) area;
    int blue = (double) getSum('b', ul, dim) / (double) area;
    int green = (double) getSum('g', ul, dim) / (double) area;

    // this creates a new RGBAPixel
    return RGBAPixel(red, green, blue);
}