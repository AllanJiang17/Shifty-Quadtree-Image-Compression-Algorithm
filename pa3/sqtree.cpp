/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

SQtree::SQtree(PNG & imIn, double tol) {
  stats s = stats(imIn);
  pair<int ,int> first_coord = make_pair(0 ,0);
  root = buildTree(s, first_coord, imIn.width(), imIn.height(), tol);
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul, int w, int h, double tol) {

  if (w * h == 0) {
    return NULL;
  }

         Node * newRoot = new Node(s, ul, w, h);

         double smallestMaxVar = -1;

         pair<int,int> upperLeftCoord = make_pair(0,0);
         pair<int,int> upperLeftSize = make_pair(0,0);

         pair<int,int> upperRightCoord = make_pair(0,0);
         pair<int,int> upperRightSize = make_pair(0,0);        

         pair<int,int> lowerLeftCoord = make_pair(0,0);
         pair<int,int> lowerLeftSize = make_pair(0,0);

         pair<int,int> lowerRightCoord = make_pair(0,0);
         pair<int,int> lowerRightSize = make_pair(0,0);        

          if (newRoot->var <= tol) {
           return newRoot;
         } else {
           for (int vertical = 0; vertical < w; vertical++) {
             for (int horizontal = 0; horizontal < h; horizontal++) {
               
                  if (vertical == 0 && horizontal == 0) {
                     //do nothing
                  } else if (vertical == 0){ // case where we split img horizontally into two blocks
                      
                      pair<int, int> coord = make_pair(ul.first, ul.second + horizontal);
                      double upperVar = s.getVar(ul, w, horizontal);
                      double lowerVar = s.getVar(coord, w, h - horizontal);

                      double maxVar = std::max(upperVar, lowerVar);
                      if (smallestMaxVar == -1 || maxVar <= smallestMaxVar) {
                        smallestMaxVar = maxVar;
                        upperLeftCoord = ul;
                        upperLeftSize = make_pair(w, horizontal);
                        
                        lowerLeftCoord = make_pair(ul.first, ul.second + horizontal);
                        lowerLeftSize = make_pair(w, h - horizontal);

                        upperRightCoord = make_pair(0,0);
                        upperRightSize = make_pair(0,0); 

                        lowerRightCoord = make_pair(0,0);
                        lowerRightSize = make_pair(0,0);   
                        
                      }

                  } else if (horizontal == 0){ // case where we split img vertically into two blocks
          
                      pair<int, int> coord = make_pair(ul.first + vertical, ul.second);
                      double leftVar = s.getVar(ul, vertical, h);
                      double rightVar = s.getVar(coord, w - vertical, h);

                      double maxVar = std::max(leftVar, rightVar);
                      if (smallestMaxVar == -1 || maxVar <= smallestMaxVar) {
                        smallestMaxVar = maxVar;
                        upperLeftCoord = ul;
                        upperLeftSize = make_pair(vertical, h);
                        
                        upperRightCoord = make_pair(ul.first + vertical, ul.second);
                        upperRightSize = make_pair(w - vertical, h);

                        lowerLeftCoord = make_pair(0,0);
                        lowerLeftSize = make_pair(0,0);

                        lowerRightCoord = make_pair(0,0);
                        lowerRightSize = make_pair(0,0);
                       
                      }

                  } else { // case where we split img into 4 blocks
                      pair<int, int> upperLeft = ul;
                      pair<int, int> upperRight = make_pair(ul.first + vertical, ul.second);
                      pair<int, int> lowerLeft = make_pair(ul.first, ul.second + horizontal);
                      pair<int, int> lowerRight = make_pair(ul.first + vertical, ul.second + horizontal);
                      double upperLeftVar = s.getVar(upperLeft, vertical, horizontal);
                      double upperRightVar = s.getVar(upperRight, w - vertical, horizontal);
                      double lowerLeftVar = s.getVar(lowerLeft, vertical, h - horizontal);
                      double lowerRightVar = s.getVar(lowerRight, w - vertical, h - horizontal);

                      double max1 = max(upperLeftVar, upperRightVar);
                      double max2 = max(lowerLeftVar, lowerRightVar);
                      double maxVar = max(max1, max2);
                      if (smallestMaxVar == -1 || maxVar <= smallestMaxVar) {
                        smallestMaxVar = maxVar;

                        upperLeftCoord = ul;
                        upperLeftSize = make_pair(vertical, horizontal);
                        
                        upperRightCoord = make_pair(ul.first + vertical, ul.second);
                        upperRightSize = make_pair(w - vertical, horizontal);
                        
                        lowerLeftCoord = make_pair(ul.first, ul.second + horizontal);
                        lowerLeftSize = make_pair(vertical, h - horizontal);
                        
                        lowerRightCoord = make_pair(ul.first + vertical, ul.second + horizontal);
                        lowerRightSize = make_pair(w - vertical, h - horizontal);
                       
                      }
                  }
             }
           }
         }
         //call recursion on nodes sub nodes with the correct inputs for pair, w and height obtained for the variance 
         newRoot -> NW = buildTree(s, upperLeftCoord, upperLeftSize.first, upperLeftSize.second, tol);
         newRoot -> NE = buildTree(s, upperRightCoord, upperRightSize.first, upperRightSize.second, tol);
         newRoot -> SW = buildTree(s, lowerLeftCoord, lowerLeftSize.first, lowerLeftSize.second, tol);
         newRoot -> SE = buildTree(s, lowerRightCoord, lowerRightSize.first, lowerRightSize.second, tol);

         return newRoot; //lastly return newRoot
}

/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  PNG image = PNG(root->width, root->height);
  renderHelper(image, root);
  return image;
}

void SQtree::renderHelper(PNG & img, Node * treeNode) {
  if (treeNode != NULL) {
  if (treeNode->NW == NULL && treeNode->NE == NULL && treeNode->SW == NULL && treeNode->SE == NULL) {
    for (int x = treeNode->upLeft.first ; x < treeNode->upLeft.first + treeNode->width; x++ ) {
      for (int y = treeNode->upLeft.second ; y < treeNode->upLeft.second + treeNode->height; y++ ) {
            RGBAPixel * pixel = img.getPixel(x,y);
            *pixel = treeNode->avg;
      }
    }
  } else {
    renderHelper(img, treeNode->NW);
    renderHelper(img, treeNode->NE);
    renderHelper(img, treeNode->SW);
    renderHelper(img, treeNode->SE);
  }
  }
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  clearHelper(root);
  root = NULL;
}

void SQtree::clearHelper(Node* treeNode) {
    if (treeNode != NULL) {
    Node* r0 = treeNode -> NE;
    Node* r1 = treeNode -> NW;
    Node* r2 = treeNode -> SW;
    Node* r3 = treeNode -> SE;
    delete treeNode;
    treeNode = NULL;
    clearHelper(r0);
    clearHelper(r1);
    clearHelper(r2);
    clearHelper(r3);
  }
}

void SQtree::copy(const SQtree & other) {
    root = copyNode(other.root);
}

SQtree::Node* SQtree::copyNode(Node* root1) {
  if (root1 == NULL) {
    return NULL;
  }

  Node* node = new Node(root1->upLeft, root1->width, root1->height, root1->avg, root1->var);
  node->NW = copyNode(root1->NW);
  node->NE = copyNode(root1->NE);
  node->SW = copyNode(root1->SW);
  node->SE = copyNode(root1->SE);
  return node; 
}

int SQtree::size() {
  return totalSize(root);
}

int SQtree::totalSize(Node * root1) {
  if(root1 == NULL) {
    return 0;
  } else {
    return 1 + totalSize(root1->NW) + totalSize(root1->NE) + totalSize(root1->SW) + totalSize(root1->SE);
  }
}