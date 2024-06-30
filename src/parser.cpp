/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "parser.h"
// #include <iostream>

//Write your code below this line
// TODO: Did we use to_string anywhere?

// Helper function declarartion
void delete_subtree(ExprTreeNode * root);
void parse_subtree(ExprTreeNode * root, vector<string> &expression, int ptr1, int ptr2);
int string_to_int(std::string str);


// Constructors and destroyers
Parser::Parser(){
    symtable = new SymbolTable();
}

Parser::~Parser(){
    int size = expr_trees.size();
    for (int i = 0; i < size; i++){delete_subtree(expr_trees[i]);}
    delete symtable;
}

// Parsing an expression
void Parser::parse(vector<string> expression){
    ExprTreeNode * root = new ExprTreeNode("ASSGN", 0);
    root->right = new ExprTreeNode();
    root->left = new ExprTreeNode();

    // int size = expression.size();
    // for (int i = 0; i < size; i++){
    //     std::cout << expression[i] << ' ' << std::flush;
    // }

    if (expression[0] == "del"){
        root->left->type = "DEL"; root->right->id = expression[2];
        root->right->type = "VAR";
        expr_trees.push_back(root);
        last_deleted = symtable->search(root->right->id);
        symtable->remove(root->right->id);
        return;
    }

    else if (expression[0] == "ret"){root->left->type = "RET";}

    else {
        root->left->id = expression[0]; root->left->type = "VAR";
        symtable->insert(root->left->id);
    }

    parse_subtree(root->right, expression, 2, expression.size() - 1);
    expr_trees.push_back(root);
}


// Helper function definition 
// Deletes the subtree starting from the root
void delete_subtree(ExprTreeNode * root){
    if (root == nullptr){return;}
    delete_subtree(root->left); delete_subtree(root->right);
    delete root;
}

// Parses the subtree recursively
void parse_subtree(ExprTreeNode * root, vector<string> &expression, int ptr1, int ptr2){
    int brac_count = 0; string oper; int oper_idx = 0;

    if (ptr1 == ptr2){
        bool pure_num = true; string token = expression[ptr1];
        int size = token.size(); int ascii;
        ascii = (int)token[0];
        if ((ascii >= 58 || ascii <= 47) && ascii != 45){pure_num = false;}
        for (int i = 1; i < size; i++){
            ascii = (int)token[i];
            if (ascii >= 58 || ascii <= 47){pure_num = false;}
        }
        if (pure_num){
            root->type = "VAL"; root->num = string_to_int(token);
        }
        else {
            root->type = "VAR"; root->id = token; 
        }
        return;
    }

    for (int i = ptr1; i < ptr2 + 1; i++){
        if (expression[i] == "("){brac_count++;}
        else if (expression[i] == ")"){brac_count--;}
        if (brac_count == 1){
            if (expression[i] == "+"){root->type = "ADD"; oper_idx = i; break;}
            else if (expression[i] == "-"){root->type = "SUB"; oper_idx = i; break;}
            else if (expression[i] == "*"){root->type = "MUL"; oper_idx = i; break;}
            else if (expression[i] == "/"){root->type = "DIV"; oper_idx = i; break;}
        }
    }

    oper = root->type;
    if (oper == ""){
        // Double brackets case
        parse_subtree(root, expression, ptr1 + 1, ptr2 - 1); return;
    }
    else {
        // Well-formed oper Well-formed
        root->left = new ExprTreeNode; root->right = new ExprTreeNode;
        parse_subtree(root->left, expression, ptr1 + 1, oper_idx - 1);
        parse_subtree(root->right, expression, oper_idx + 1, ptr2 - 1); return;
    }
}

// Helper function which converts string to int
int string_to_int(std::string str){
    if (str[0] == '0'){return 0;}
    bool neg_sign = (str[0] == '-'); int size = str.size();
    int num = 0;
    for (int i = neg_sign; i < size; i++){
        num = num*10; num += str[i]-48;
    }
    if (neg_sign){num = num*(-1);}
    return num;
}
