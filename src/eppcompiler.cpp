/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "eppcompiler.h"

//Write your code below this line

// Helper function declaration
void create_commands(ExprTreeNode * expr_root,vector<string> &commands, MinHeap &least_mem_loc, Parser &targ);
void recursive_commands(ExprTreeNode * expr_root, vector<string> &commands, Parser &targ);

// Constructors and Destroyers
EPPCompiler::EPPCompiler(){
    memory_size = 0; output_file = "";
    mem_loc.resize(memory_size);
    for (int i = 0; i < memory_size; i++){
        least_mem_loc.push_heap(i);
    }
}

EPPCompiler::EPPCompiler(string out_file,int mem_limit){
    memory_size = mem_limit; output_file = out_file;
    mem_loc.resize(memory_size);
    for (int i = 0; i < memory_size; i++){
        least_mem_loc.push_heap(i);
    }
}

EPPCompiler::~EPPCompiler(){}

// Compile function
void EPPCompiler::compile(vector<vector<string>> code){
    int num_commands = code.size();
    vector<string> commands_to_write;
    for (int i = 0; i < num_commands; i++){
        targ.parse(code[i]);
        vector<string> commands = generate_targ_commands();
        int size = commands.size();
        for (int j = 0; j < size; j++){
            commands_to_write.push_back(commands[j]);   
        }
    }

    write_to_file(commands_to_write);
}

// Generates the commands for compiling into targ language.
vector<string> EPPCompiler::generate_targ_commands(){
    vector<string> commands;
    vector<ExprTreeNode*> &exprs = targ.expr_trees;
    int size = exprs.size();
    create_commands(exprs[size - 1], commands, least_mem_loc, targ);
    return commands;
}

// Writes the generated commands onto a file
void EPPCompiler::write_to_file(vector<string> commands){
    ofstream my_file;
    my_file.open(output_file);
    int size = commands.size();
    for (int i = 0; i < size; i++){
        my_file << commands[i] << std::endl;
    }
    my_file.close();
}

// Helper function definitions
void create_commands(ExprTreeNode * expr_root,vector<string> &commands, MinHeap &least_mem_loc, Parser &targ){

    if (expr_root->left->type == "RET"){
        // Return value
        recursive_commands(expr_root->right, commands, targ);
        commands.push_back("RET = POP");
    }
    else if (expr_root->left->type == "VAR"){
        // Assigment
        recursive_commands(expr_root->right, commands, targ);
        int mem_loc = targ.symtable->search(expr_root->left->id);
        if (mem_loc == -1){
            mem_loc = least_mem_loc.get_min(); least_mem_loc.pop();
            targ.symtable->assign_address(expr_root->left->id, mem_loc);
        }
        commands.push_back("mem[" + std::to_string(mem_loc) + "] = POP");
    }
    else{
        // Delete
        int mem_loc = targ.last_deleted;
        if (mem_loc != -2){
            commands.push_back("DEL = mem[" + std::to_string(mem_loc) + "]");
            least_mem_loc.push_heap(mem_loc);
        }
    }
}


void recursive_commands(ExprTreeNode * expr_root, vector<string> &commands, Parser &targ){
    if (expr_root == NULL){return;}
    recursive_commands(expr_root->right, commands, targ);
    recursive_commands(expr_root->left, commands, targ);

    if (expr_root->type == "VAR"){
        int mem_loc = targ.symtable->search(expr_root->id);
        if (mem_loc >= 0){
            commands.push_back("PUSH mem[" + std::to_string(mem_loc) + "]");
        }
    }
    else if (expr_root->type == "VAL"){
        commands.push_back("PUSH " + std::to_string(expr_root->num));
    }
    else {
        commands.push_back(expr_root->type);
    }
}