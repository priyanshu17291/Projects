/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "entry.h"

SymEntry::SymEntry(){
    key = "";
    UnlimitedRational* temp = new UnlimitedRational();
    val = temp;
    left = nullptr;
    right = nullptr;
}

SymEntry::SymEntry(string k, UnlimitedRational* v){
    key = k;
    val = v;
    left = nullptr;
    right = nullptr;
}

SymEntry::~SymEntry(){
    val = nullptr;
    left = nullptr;
    right = nullptr;
    delete val;
    delete left;
    delete right;
}