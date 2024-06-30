/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "minheap.h"

// Helper functions declaration
void delete_subtree(HeapNode * root);
void create_new_level(HeapNode * root,int num);
void heapify_up(HeapNode * child);
void heapify_down(HeapNode * root);
HeapNode * create_new_node(HeapNode * root, int size, int helper);
HeapNode * get_last_node(HeapNode * root, int size, int helper);
int get_helper(int size);

// Constructors and Destroyers
MinHeap::MinHeap(){
    root = NULL; size = 0;
}

MinHeap::~MinHeap(){delete_subtree(root);}

// Dynamic operations - pushing and popping
void MinHeap::push_heap(int num){
    if (size == 0){root = new HeapNode(num); size++; return;}
    int helper = get_helper(size);
    
    // Check for new level
    if (size == (4*helper - 1) || size == 1){create_new_level(root, num); size++; return;}

    HeapNode * last_node = create_new_node(root, size, helper);
    last_node->val = num; heapify_up(last_node);

    size++;
}

// Pops the minimum element
void MinHeap::pop(){
    if (size == 1){delete root; root = NULL; size--; return;}
    int helper = get_helper(size);

    HeapNode * temp_node = get_last_node(root, size, helper);
    int root_val = root->val; int node_val = temp_node->val;
    root->val = node_val; temp_node->val = root_val; 

    HeapNode * parent_node = temp_node->par;
    if (parent_node->right == temp_node){parent_node->right = NULL;}
    else {parent_node->left = NULL;}
    delete temp_node;

    heapify_down(root);
    size--;
}

// Static operations - Getting minimum
int MinHeap::get_min(){return root->val;}

// Helper functions

// Deletes the subtree
void delete_subtree(HeapNode * root){
    if (root == NULL){return;}
    delete_subtree(root->left);
    delete_subtree(root->right);
    delete root;
}

// Inserts a new level
void create_new_level(HeapNode * root,int num){
    if (root->left == NULL){
        root->left = new HeapNode(num); root->left->par = root; 
        heapify_up(root->left);
        return;
    }
    create_new_level(root->left, num);
}

// Heapifys a node down and up the heap. Useful for maintaining heap data structure
void heapify_down(HeapNode * root){
    if (root->left != NULL && root->right != NULL){
        int left = root->left->val, right = root->right->val, val = root->val;
        if (val <=  left && val <= right){return;}
        else if (left <= val && left <= right){
            root->val = left; root->left->val = val; return heapify_down(root->left);
        }
        else {
            root->val = right; root->right->val = val; return heapify_down(root->right);
        }
    }
    else if (root->left != NULL){
        int val = root->val; int left = root->left->val;
        if (left < val){root->val = left; root->left->val = val; heapify_down(root->left);}
        else {return;}
    }
    else{return;}
}

void heapify_up(HeapNode * root){
    if (root->par == NULL){return;}
    int root_val = root->val, parent_val = root->par->val;
    if (root_val > parent_val){return;}
    else {root->val = parent_val; root->par->val = root_val; heapify_up(root->par);}
}

// Creates new node while maintaining the heap property and
// Returns a pointer to it. Helper is 2**(h-1)
HeapNode * create_new_node(HeapNode * root, int size, int helper){
    // TODO: Is Code fucked while creating a new level?
    if (size == 1){
        root->left = new HeapNode(); root->left->par = root; return root->left;
    }
    else if (size == 2){
        root->right = new HeapNode(); root->right->par = root; return root->right;
    }
    int compare = 3*helper - 1;
    if (size >= compare){return create_new_node(root->right, size - 2*helper, helper/2);}
    else{return create_new_node(root->left, size - helper, helper/2);}
}

// Returns the "last node" BFS traversal and returns it
// Helper is 2**(h-1)
HeapNode * get_last_node(HeapNode * root, int size, int helper){
    // TODO: Is Code fucked while deleting a level
    if (size == 1){return root;}
    int compare = 3*helper;
    if (size >= compare){return get_last_node(root->right, size - 2*helper, helper/2);}
    else{return get_last_node(root->left, size - helper, helper/2);}
}

// Returns the helper variable given the size of the tree
// Helper variable is 2**(h-1)
int get_helper(int size){
    int height = 0, helper = 1;
    while (size != 1){size = size/2; height++;}
    for (int i = 1; i < height; i++){helper = helper*2;}
    return helper;
}
