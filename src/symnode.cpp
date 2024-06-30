/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symnode.h"

//Write your code below this line

// Constructors and destroyers
SymNode::SymNode(){
    height = 0; key = ""; address = -1;
    left = NULL; right = NULL; par = NULL;
}

SymNode::SymNode(string k){
    height = 0; key = k; address = -1;
    left = NULL; right = NULL; par = NULL;
}

SymNode::~SymNode(){}

// Rotation operations
SymNode* SymNode::LeftLeftRotation(){
    SymNode * parent = this->par; SymNode * right_node = this->right;
    right_node->par = parent; this->right = right_node->left;
    if (right_node->left){right_node->left->par = this;}
    right_node->left = this; this->par = right_node;
    if (parent){
        if (parent->right){
            if (parent->right->key == this->key){parent->right = right_node;}
            else {parent->left = right_node;}
        }
        else{parent->left = right_node;}
    }
    return right_node;
}

SymNode* SymNode::RightRightRotation(){
    SymNode * parent = this->par; SymNode * left_node = this->left;
    left_node->par = parent; this->left = left_node->right;
    if (left_node->right){left_node->right->par = this;}
    left_node->right = this; this->par = left_node;
    if (parent){
        if (parent->right){
            if (parent->right->key == this->key){parent->right = left_node;}
            else {parent->left = left_node;}
        }
        else{parent->left = left_node;}
    }
    return left_node;
}

SymNode* SymNode::LeftRightRotation(){
    this->left->LeftLeftRotation(); return this->RightRightRotation();
}

SymNode* SymNode::RightLeftRotation(){
    this->right->RightRightRotation(); return this->LeftLeftRotation();
}

