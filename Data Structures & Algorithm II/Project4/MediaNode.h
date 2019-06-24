
#ifndef MediaNode_h
#define MediaNode_h

#include <iostream>
#include <string>
#include <algorithm>
#include "Media.h"
using namespace std;

//template <class T>
class MediaNode {
private:
	Media * data;
	MediaNode * left;
	MediaNode * right;
public:
	MediaNode();
	MediaNode(Media * data);
	MediaNode(Media * data, MediaNode* l, MediaNode * r);
	Media * getData();
	MediaNode * getLeft();
	MediaNode * getRight();
	void setData(Media * data);
	void setLeft(MediaNode * left);
	void setRight(MediaNode * right);
};

#endif