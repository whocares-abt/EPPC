/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"

//Write your code below this line

// Constructors and destroyers
ExprTreeNode::ExprTreeNode(){
    left = NULL; right = NULL; type = ""; num = 0; id = "";
} 

ExprTreeNode::ExprTreeNode(string t,int v){
    left = NULL; right = NULL; type = t; num = v; id = "";
}

ExprTreeNode::~ExprTreeNode(){}
