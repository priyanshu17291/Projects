/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "evaluator.h"
#include <iostream>
using namespace std;

Evaluator::Evaluator()
{
    symtable = new SymbolTable();
}

Evaluator::~Evaluator()
{
    for (long unsigned i = 0; i < expr_trees.size(); i++)
    {
        expr_trees[i] = nullptr;
        delete expr_trees[i];
    }
    expr_trees.clear();
    symtable = nullptr;
    delete symtable;
}

bool isOperator(string s){
    if(s=="+" || s =="-"|| s=="*" || s =="/") return true;
    return false;
}


void Evaluator::parse(vector<string> code)
{
    UnlimitedInt *temp = new UnlimitedInt("0");
    UnlimitedInt *temp1 = new UnlimitedInt("0");
    ExprTreeNode *root = new ExprTreeNode(code[1], temp);
    root->left = new ExprTreeNode("VAR", temp1);
    root->left->id = code[0];

    ExprTreeNode *rightNode = nullptr;

    vector<ExprTreeNode *> stk;
    int s = 0;
    int i = 2;

    while (i < (int)code.size())
    {

        if (code[i] == "(")
        {
            i++;
            string next = code[i];

            if (next == "("){
                continue;
            }
            UnlimitedInt *temp = new UnlimitedInt("0");
            ExprTreeNode *node = new ExprTreeNode(next, temp);
            stk.push_back(node);
            s++;
            
            i++;
        }
        else if (code[i] == ")")
        {
            ExprTreeNode *temp = stk[s - 1];
            stk.pop_back();
            s--;
            ExprTreeNode *node = stk[s - 1];
            stk.pop_back();
            s--;

            node->right = temp;
            stk.push_back(node);
            s++;
            i++;
        }
        else if (isOperator(code[i]))
        {
            ExprTreeNode *node;
            UnlimitedInt *temp = new UnlimitedInt("0");
            node = new ExprTreeNode(code[i], temp);
            node->left = stk[s - 1];
            stk.pop_back();
            stk.push_back(node);
            i++;
        }
        else
        {
            UnlimitedInt *temp = new UnlimitedInt("0");
            ExprTreeNode *node = new ExprTreeNode(code[i], temp);
            if (s == 0)
            {
                rightNode = node;
                break;
            }

            stk.push_back(node);
            s++;
            i++;
        }
    }

    if (rightNode == nullptr)
    {
        rightNode = stk[0];
        stk.pop_back();
    }

    root->right = rightNode;
    expr_trees.push_back(root);
}

UnlimitedRational *evaluate(ExprTreeNode *root, SymbolTable *&symtable)
{
    if (root->left == nullptr && root->right == nullptr)
    {
        if (root->type == "VAL")
        {
            root->evaluated_value = root->val;
            return root->evaluated_value;
        }
        else
        {
            root->evaluated_value = symtable->search(root->id);
            return root->evaluated_value;
        }
    }
    else if (root->type == "ADD")
    {
        root->evaluated_value = UnlimitedRational::add(evaluate(root->left, symtable), evaluate(root->right, symtable));
        return root->evaluated_value;
    }
    else if (root->type == "SUB")
    {
        root->evaluated_value = UnlimitedRational::sub(evaluate(root->left, symtable), evaluate(root->right, symtable));
        return root->evaluated_value;
    }
    else if (root->type == "MUL")
    {
        root->evaluated_value = UnlimitedRational::mul(evaluate(root->left, symtable), evaluate(root->right, symtable));
        return root->evaluated_value;
    }
    else
    {
        root->evaluated_value = UnlimitedRational::div(evaluate(root->left, symtable), evaluate(root->right, symtable));
        return root->evaluated_value;
    }
}

void Evaluator::eval()
{
    long unsigned j = expr_trees.size();
    ExprTreeNode *node = expr_trees[j - 1];
    node->evaluated_value = evaluate(node->right, symtable);
    symtable->insert(node->left->id, node->evaluated_value);
}