#include "stats.h"


#include "stats.h"

stats::stats(PNG & im){

unsigned int width = im.width();
unsigned int height = im.height();

sumRed.resize(width);
sumGreen.resize(width);
sumBlue.resize(width);
sumsqRed.resize(width);
sumsqGreen.resize(width);
sumsqBlue.resize(width);

for(unsigned int i = 0; i < width; i++) {

    sumRed[i].resize(height);
    sumGreen[i].resize(height);
    sumBlue[i].resize(height);
    sumsqRed[i].resize(height);
    sumsqGreen[i].resize(height);
    sumsqBlue[i].resize(height);

    for(unsigned int j = 0; j < height; j++) {
        RGBAPixel * pixel = im.getPixel(i,j);
        if (i==0 && j==0) {
            sumRed[i][j] = pixel -> r;
            sumGreen[i][j] = pixel -> g;
            sumBlue[i][j] = pixel -> b;

            sumsqRed[i][j] = pow(pixel->r, 2.0);
            sumsqGreen[i][j] = pow(pixel->g, 2.0);
            sumsqBlue[i][j] = pow(pixel->b, 2.0);
        } else if(i==0) {
            sumRed[i][j] = sumRed[i][j-1] + pixel->r;
            sumGreen[i][j] = sumGreen[i][j-1] + pixel->g;
            sumBlue[i][j] = sumBlue[i][j-1] + pixel->b;

            sumsqRed[i][j] = sumsqRed[i][j-1] + pow(pixel->r, 2.0);
            sumsqGreen[i][j] = sumsqGreen[i][j-1] + pow(pixel->g, 2.0);
            sumsqBlue[i][j] = sumsqBlue[i][j-1] + pow(pixel->b, 2.0);
        } else if(j==0) {
            sumRed[i][j] = sumRed[i-1][j] + pixel->r;
            sumGreen[i][j] = sumGreen[i-1][j] + pixel->g;
            sumBlue[i][j] = sumBlue[i-1][j] + pixel->b;

            sumsqRed[i][j] = sumsqRed[i-1][j] + pow(pixel->r, 2.0);
            sumsqGreen[i][j] = sumsqGreen[i-1][j] + pow(pixel->g, 2.0);
            sumsqBlue[i][j] = sumsqBlue[i-1][j] + pow(pixel->b, 2.0);
        } else {
            sumRed[i][j] = sumRed[i-1][j] + sumRed[i][j-1] - sumRed[i-1][j-1] + pixel->r;
            sumGreen[i][j] = sumGreen[i-1][j] + sumGreen[i][j-1] - sumGreen[i-1][j-1] + pixel->g;
            sumBlue[i][j] = sumBlue[i-1][j] + sumBlue[i][j-1] - sumBlue[i-1][j-1] + pixel->b;

            sumsqRed[i][j] = sumsqRed[i-1][j] + sumsqRed[i][j-1] - sumsqRed[i-1][j-1] + pow(pixel->r, 2.0);
            sumsqGreen[i][j] = sumsqGreen[i-1][j] + sumsqGreen[i][j-1] - sumsqGreen[i-1][j-1] + pow(pixel->g, 2.0);
            sumsqBlue[i][j] = sumsqBlue[i-1][j] + sumsqBlue[i][j-1] - sumsqBlue[i-1][j-1] + pow(pixel->b, 2.0);
        }
    }
}
    
}

long stats::getSum(char channel, pair<int,int> ul, int w, int h){
        int x = ul.first - 1;
        int y = ul.second - 1;
        long largeRec = 0;
        long lowerLeft = 0;
        long upperRight = 0;
        long extraArea = 0;
    if (channel == 'r') {
            largeRec = sumRed[ul.first + w - 1][ul.second + h - 1];

        if (x >= 0) {
            lowerLeft = sumRed[x][ul.second + h - 1];
        }

        if (y >= 0) {
            upperRight = sumRed[ul.first + w - 1][y];
        }

        if (x >= 0 && y >= 0) {
            extraArea = sumRed[x][y];
        }
        

    } else if (channel == 'g') {
            largeRec = sumGreen[ul.first + w - 1][ul.second + h - 1];

        if (x >= 0) {
             lowerLeft = sumGreen[x][ul.second + h - 1];
        }

        if (y >= 0) {
             upperRight = sumGreen[ul.first + w - 1][y];
        }

        if (x >= 0 && y >= 0) {
             extraArea = sumGreen[x][y];
        }
        

    } else if (channel == 'b') {
            largeRec = sumBlue[ul.first + w - 1][ul.second + h - 1];

        if (x >= 0) {
           lowerLeft = sumBlue[x][ul.second + h - 1];
        }

        if (y >= 0) {
            upperRight = sumBlue[ul.first + w - 1][y];
        }

        if (x >= 0 && y >= 0) {
           extraArea = sumBlue[x][y];
        }
        
    }
    return largeRec - upperRight - lowerLeft + extraArea;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
        int x = ul.first - 1;
        int y = ul.second - 1;
        long largeRec = 0;
        long lowerLeft = 0;
        long upperRight = 0;
        long extraArea = 0;

    if (channel == 'r') {
        largeRec = sumsqRed[ul.first + w - 1][ul.second + h - 1];

        if (x >= 0) {
            lowerLeft = sumsqRed[x][ul.second + h - 1];
        }

        if (y >= 0) {
            upperRight = sumsqRed[ul.first + w - 1][y];
        }

        if (x >= 0 && y >= 0) {
           extraArea = sumsqRed[x][y];
        }
        

    } else if (channel == 'g') {
             largeRec = sumsqGreen[ul.first + w - 1][ul.second + h - 1];

        if (x >= 0) {
             lowerLeft = sumsqGreen[x][ul.second + h - 1];
        }

        if (y >= 0) {
             upperRight = sumsqGreen[ul.first + w - 1][y];
        }

        if (x >= 0 && y >= 0) {
           extraArea = sumsqGreen[x][y];
        }

    } else if (channel == 'b') {
             largeRec = sumsqBlue[ul.first + w - 1][ul.second + h - 1];

       if (x >= 0) {
            lowerLeft = sumsqBlue[x][ul.second + h - 1];
        }

        if (y >= 0) {
            upperRight = sumsqBlue[ul.first + w - 1][y];
        }
        if (x >= 0 && y >= 0) {
           extraArea = sumsqBlue[x][y];
        }
    
    }
    return largeRec -  upperRight - lowerLeft + extraArea;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    double redVar = getSumSq('r', ul, w, h) -  (pow(getSum('r', ul, w, h), 2) / (double) (w*h));
    double greenVar = getSumSq('g', ul, w, h) - (pow(getSum('g', ul, w, h), 2) / (double) (w*h));
    double blueVar = getSumSq('b', ul, w, h) - (pow(getSum('b', ul, w, h), 2) / (double) (w*h));

    return (double) (redVar + greenVar + blueVar);
}

RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
 int red = (getSum('r', ul, w, h)/(w*h));
 int green = (getSum('g', ul, w, h)/(w*h));
 int blue = (getSum('b', ul, w, h)/(w*h));

return RGBAPixel(red, green, blue);
}