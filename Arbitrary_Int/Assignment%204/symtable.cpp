/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"

SymbolTable::SymbolTable(){
    size = 0;
    root = nullptr;
}

void Delete(SymEntry* root){
    if(root == nullptr){
        return;
    }
    else if(root->left == nullptr && root->right==nullptr){
        delete root;
        return;
    }
    else{
        Delete(root->left);
        Delete(root->right);
        delete root;
        return;
    }
}

SymbolTable::~SymbolTable(){
    Delete(root);
}

void SymbolTable::insert(string k, UnlimitedRational* v){
    if (!root) {
        root = new SymEntry(k,v);
        size++;
        return;
    }
    SymEntry* temp =  root;
    while(temp->left != nullptr || temp->right != nullptr){
        if(k < temp->key){
            if(temp->left == nullptr){
                temp->left = new SymEntry(k,v);
                size++;
                return;
            }
            temp = temp->left;
        }
        else{
            if(temp->right == nullptr){
                temp->right = new SymEntry(k,v);
                size++;
                return;
            }
            temp = temp->right;
        }
    }
    if(k < temp->key){
        temp->left = new SymEntry(k,v);
        size++;
        return;
    }
    else{
        temp->right = new SymEntry(k,v);
        size++;
        return;
    }
}

void SymbolTable::remove(string k){
    if (!root) {
        return;
    }
    SymEntry* temp =  root;
    while(temp->left != nullptr && temp->left != nullptr){
        if(k < temp->key){
            temp = temp->left;
        }
        else if (k > temp->key){
            temp = temp->right;
        }
        else{
            temp->val = nullptr;
            size--;
            return;
        }
    }
}

UnlimitedRational* SymbolTable::search(string k){
    if (root->key == k) {
        return root->val;
    }
    SymEntry* temp =  root;
    while(temp != nullptr ){
        if(k < temp->key){
            temp = temp->left;
        }
        else if (k > temp->key){
            temp = temp->right;
        }
        else{
            return temp->val;
        }
    }
    return nullptr;
}

int SymbolTable::get_size(){
    return size;
}

SymEntry* SymbolTable::get_root(){
    return root;
}