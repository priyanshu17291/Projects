/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"


ExprTreeNode::ExprTreeNode(){
    val=nullptr;
    evaluated_value=nullptr;
    left=nullptr;
    right=nullptr;
}

bool is_digits1(string& str){
    if(str[0] == '-'){
        for (long unsigned i = 1 ; i< str.size() ; i++) {
            char ch = str[i];
            if (!(ch >= 48 && ch <= 57)) {
                return false;
            }
        }
    }
    else{
    for (long unsigned i = 0 ; i< str.size() ; i++) {
        char ch = str[i];
            if (!(ch >= 48 && ch <= 57)) {
                return false;
            }
        }
    }
    return true;
}

ExprTreeNode::ExprTreeNode(string t, UnlimitedInt* v){
    left=nullptr;
    right=nullptr;
    UnlimitedInt* temp = new UnlimitedInt(1);
    if (t == "+"){
        type = "ADD";
        val = nullptr;
        evaluated_value = new UnlimitedRational(v,temp);
    }
    else if (t == "-"){
        type = "SUB";
        val = nullptr;
        evaluated_value = new UnlimitedRational(v,temp);
    }
    else if (t == "*"){
        type = "MUL";
        val = nullptr;
        evaluated_value = new UnlimitedRational(v,temp);
    }
    else if (t == "/"){
        type = "DIV";
        val = nullptr;
        evaluated_value = new UnlimitedRational(v,temp);
    }
    else if (is_digits1(t)){
        type = "VAL";
        UnlimitedInt* num = new UnlimitedInt(t);
        val = new UnlimitedRational(num,temp);
        evaluated_value = new UnlimitedRational(v,temp);
    }
    else{
        type = "VAR";
        id = t;
        val = nullptr;
        evaluated_value = new UnlimitedRational(v,temp);
    }

}

ExprTreeNode::ExprTreeNode(string t, UnlimitedRational* v){
    left=nullptr;
    right=nullptr;
    if (t == "+"){
        type = "ADD";
        val = nullptr;
        evaluated_value = v;
    }
    else if (t == "-"){
        type = "SUB";
        val = nullptr;
        evaluated_value = v;
    }
    else if (t == "*"){
        type = "MUL";
        val = nullptr;
        evaluated_value = v;
    }
    else if (t == "/"){
        type = "DIV";
        val = nullptr;
        evaluated_value = v;
    }
    else if (is_digits1(t)){
        type = "VAL";
        UnlimitedInt* temp = new UnlimitedInt(1);
        UnlimitedInt* num = new UnlimitedInt(t);
        val = new UnlimitedRational(num,temp);
        evaluated_value = v;
    }
    else{
        type = "VAR";
        id = t;
        val = nullptr;
        evaluated_value = v;
    }

}

ExprTreeNode::~ExprTreeNode(){
    val=nullptr;
    evaluated_value=nullptr;
    left=nullptr;
    right=nullptr;
    delete val;
    delete evaluated_value;
    delete left;
    delete right;
}
