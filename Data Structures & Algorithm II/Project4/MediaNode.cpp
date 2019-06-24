
#include "MediaNode.h"

//template <class T>
MediaNode::MediaNode() {
	//this->data = &Media();
	this->data = nullptr;
	this->left = nullptr;
	this->right = nullptr;
}

//template <class T>
MediaNode::MediaNode(Media * data) {
	this->data = data;
	this->left = nullptr;
	this->right = nullptr;
}

//template <class T>
MediaNode::MediaNode(Media * data, MediaNode * l, MediaNode * r) {
	this->data = data;
	this->left = l;
	this->right = r;
}

//template <class T>
Media * MediaNode::getData() {
	return data;
}

//template <class T>
MediaNode * MediaNode::getRight() {
	return this->right;
}

//template <class T>
MediaNode * MediaNode::getLeft() {
	return this->left;
}

//template <class T>
void MediaNode::setData(Media * data) {
	this->data = data;
}

//template <class T>
void MediaNode::setLeft(MediaNode * left) {
	this->left = left;
}

//template <class T>
void MediaNode::setRight(MediaNode * right) {
	this->right = right;
}
