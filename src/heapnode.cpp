/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "heapnode.h"

//Write your code below this line

// Constructors and destroyers
HeapNode::HeapNode(){
    par = NULL; left = NULL; right = NULL; val = 0;
}

HeapNode::HeapNode(int _val){
    par = NULL; left = NULL; right = NULL; val = _val;
}

HeapNode::~HeapNode(){}
