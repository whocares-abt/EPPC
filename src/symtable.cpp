/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"

//Write your code below this line

// Helper function declaration
void delete_subtree(SymNode * root);

SymNode * find_node(SymNode * root, string k);
SymNode * insert_new_node(SymNode * root, string k);
SymNode * displace_node(SymNode * node);

SymNode * get_successor_right(SymNode * node);
SymNode * get_predecessor_left(SymNode * node);

void update_height(SymNode * root);
int get_skew(SymNode * root);
SymNode * adjust_skew(SymNode * root);
SymNode * update_AVL(SymNode * root);

// Constructors and Destructors;
SymbolTable::SymbolTable(){root = NULL; size = 0;}

SymbolTable::~SymbolTable(){delete_subtree(root); size = 0;}

// Dynamic operations - Insert and remove
void SymbolTable::insert(string k){
    if (size == 0){
        root = new SymNode(k); size++; 
        return;
    }
    if (find_node(root, k)){return;}
    SymNode * new_node = insert_new_node(root, k);
    SymNode * parent = new_node->par;
    root = update_AVL(parent);
    size++;
}

void SymbolTable::remove(string k){
    if (size){
        if (size == 1){
            if (root->key == k){delete root; size--;}
            return;
        }
        SymNode * node = find_node(root, k);
        if (node == NULL){return;}
        SymNode * node_tbd = displace_node(node);
        SymNode * parent = node_tbd->par;
        if (parent->left){
            if (parent->left->key == node_tbd->key){parent->left = NULL;}
            else {parent->right = NULL;}
        }
        else{parent->right = NULL;}
        delete node_tbd;
        root = update_AVL(parent);
        size--;
    }
}

// Static operations - Searching and assigning address
int SymbolTable::search(string k){
    SymNode * node = find_node(root, k);
    if (node){return node->address;}
    else {return -2;}
}

void SymbolTable::assign_address(string k,int idx){
    SymNode * node = find_node(root, k);
    if (node){node->address = idx;}
}

// Getter functions
int SymbolTable::get_size(){return size;}

SymNode* SymbolTable::get_root(){return root;}

// Helper functions

// Deletes a subtree given it's root
void delete_subtree(SymNode * root){
    if (root == NULL){return;}
    delete_subtree(root->left); delete_subtree(root->right);
    delete root;
}

// Finds a node given it's key (string).
// If node does not exist, returns NULL.
SymNode * find_node(SymNode * root, string k){
    if (root == NULL){return NULL;}
    if (root->key == k){return root;}
    else if (root->key > k){return find_node(root->left, k);}
    else {return find_node(root->right, k);}
}

// Inserts a new node and returns a pointer to it
SymNode * insert_new_node(SymNode * root, string k){
    if (root->key > k){
        if (root->left){return insert_new_node(root->left, k);}
        root->left = new SymNode(k); root->left->par = root; return root->left;
    }
    else {
        if (root->right){return insert_new_node(root->right, k);}
        root->right = new SymNode(k); root->right->par = root; return root->right;
    }
}

// Displaces the node to a leaf (so that it can be deleted)
SymNode * displace_node(SymNode * root){
    if (root->left == NULL && root->right == NULL){return root;}
    else if (root->left){
        SymNode * pre = get_predecessor_left(root);
        std::swap(pre->key, root->key);
        std::swap(pre->address, root->address);
        return displace_node(pre);
    }
    else{
        SymNode * suc = get_successor_right(root);
        std::swap(suc->key, root->key);
        std::swap(suc->address, root->address);
        return displace_node(suc);
    }
}

// Helper function for obtaining predecessor and successor

// Gives successor provided right node exists.
SymNode * get_successor_right(SymNode * node){
    SymNode * curr_node = node->right;
    while (curr_node->left){curr_node = curr_node->left;}
    return curr_node;
}

// Gives predecessor provided left node exists.
SymNode * get_predecessor_left(SymNode * node){
    SymNode * curr_node = node->left;
    while (curr_node->right){curr_node = curr_node->right;}
    return curr_node; 
}

// Helper functions for adjusting height

// Updates the height of a node wrt it's children
// Height of leaf is zero
void update_height(SymNode * root){
    int left_ht = -1, right_ht = -1;
    if (root->left){left_ht = root->left->height;}
    if (root->right){right_ht = root->right->height;}
    root->height = 1 + std::max(left_ht, right_ht);
}

// Returns the skew of a node
// Skew = node.left ht - node.right ht;
int get_skew(SymNode * root){
    int left_ht = -1, right_ht = -1;
    if (root->left){left_ht = root->left->height;}
    if (root->right){right_ht = root->right->height;}
    return (left_ht - right_ht);
}

// If skew > 1 or < -1; this adjusts the skew st AVL property is maintained
// Also updates the heights of all the nodes in the subtree ONLY.
// Parents of the subtree don't have their height updated.
SymNode * adjust_skew(SymNode * root){
    int skew = get_skew(root); SymNode * node_to_return;
    if (skew == 2){
        SymNode * left_node = root->left;
        int left_skew = get_skew(left_node);
        switch (left_skew){
            case 1:
            case 0:
                node_to_return = root->RightRightRotation();
                update_height(root); update_height(left_node); 
                return node_to_return;
            case -1:
                node_to_return = root->LeftRightRotation();
                update_height(root); update_height(left_node); update_height(root->par);
                return node_to_return;
        }
    }
    else if (skew == -2) {
        SymNode * right_node = root->right;
        int right_skew = get_skew(right_node);
        switch (right_skew){
            case -1:
            case 0:
                node_to_return = root->LeftLeftRotation();
                update_height(root); update_height(right_node);
                return node_to_return;
            case 1:
                node_to_return = root->RightLeftRotation();
                update_height(root); update_height(right_node); update_height(root->par);
                return node_to_return;
        }
    }
    return root;
}

// Updates the tree throughout so that it maintains it's AVL invariant
SymNode * update_AVL(SymNode * subtree_root){
    if (subtree_root->par == NULL){
        update_height(subtree_root);
        SymNode * new_root = adjust_skew(subtree_root);
        return new_root;
    }
    update_height(subtree_root);
    adjust_skew(subtree_root);
    return update_AVL(subtree_root->par);
}

