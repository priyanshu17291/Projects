/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "ulimitedrational.h"

UnlimitedRational::UnlimitedRational(){
    p=nullptr;
    q=nullptr;
}

int compare2(UnlimitedInt* i1 , UnlimitedInt* i2){
    if(i1->get_size()>i2->get_size()) return 1;
    else if(i1->get_size()<i2->get_size()) return -1;
    else{
        int m = i1->get_size();
        for(int i = 0 ; i < m ; i++){
            if(i1->get_array()[i] > i2->get_array()[i]) return 1;
            else if(i1->get_array()[i] < i2->get_array()[i]){
                return -1;
            }
        }
        return 0;
    }
}

UnlimitedInt* gcd(UnlimitedInt* a, UnlimitedInt* b)
{
    while (a->get_sign() > 0 && b->get_sign() > 0) {
        if (compare2(a,b)==1) {
            a = UnlimitedInt::mod(a, b);
        }
        else {
            b = UnlimitedInt::mod(b, a);
        }
    }
    if (a->get_sign() == 0) {
        return b;
    }
    return a;
}

UnlimitedRational::UnlimitedRational(UnlimitedInt* num, UnlimitedInt* den){
    if(den->get_sign()==0){
        p = new UnlimitedInt(0);
        q = new UnlimitedInt(0);
        delete num;
    }
    else if(num->get_sign()==0){
        UnlimitedInt* y = new UnlimitedInt(den->to_string());
        p = new UnlimitedInt(0);
        q = y;
    }
    else{
        int numsign = num->get_sign();
        int densign = den->get_sign();
        if(numsign == 1 && densign == 1){
            UnlimitedInt* temp = gcd(num,den);
            p = UnlimitedInt::div(num,temp);
            q = UnlimitedInt::div(den,temp);
        }
        else if(numsign == -1 && densign == -1){
            UnlimitedInt* temp = new UnlimitedInt(-1);
            UnlimitedInt* num1 = UnlimitedInt::mul(num,temp);
            UnlimitedInt* den1 = UnlimitedInt::mul(den,temp);
            temp = gcd(num1,den1);
            p = UnlimitedInt::div(num1,temp);
            q = UnlimitedInt::div(den1,temp);
            delete temp;
            delete num1;
            delete den1;
        }
        else if(numsign == -1){
            UnlimitedInt* temp = new UnlimitedInt(-1);
            UnlimitedInt* num1 = UnlimitedInt::mul(num,temp);
            UnlimitedInt* temp1 = gcd(num1,den);
            UnlimitedInt* temp2 = UnlimitedInt::div(num1,temp1);
            p = UnlimitedInt::mul(temp2,temp);
            q = UnlimitedInt::div(den,temp1);
            delete temp;
            delete temp2;
            delete num1;
        }
        else if(densign == -1){
            UnlimitedInt* temp = new UnlimitedInt(-1);
            UnlimitedInt* den1 = UnlimitedInt::mul(den,temp);
            UnlimitedInt* temp1 = gcd(den1,num);
            UnlimitedInt* temp2 = UnlimitedInt::div(den1,temp1);
            p = UnlimitedInt::div(num,temp1);
            q = UnlimitedInt::mul(temp2,temp);
            delete temp;
            delete temp2;
            delete den1;
        }
    }
}

UnlimitedRational::~UnlimitedRational(){
    p=nullptr;
    q=nullptr;
    delete p;
    delete q;
}

UnlimitedInt* UnlimitedRational::get_p(){
    return this->p;
}

UnlimitedInt* UnlimitedRational::get_q(){
    return this->q;
}

string UnlimitedRational::get_p_str(){
    string res = p->to_string();
    return res;
}

string UnlimitedRational::get_q_str(){
    string res = q->to_string();
    return res;
}

string UnlimitedRational::get_frac_str(){
    string n = p->to_string();
    string d = q->to_string();
    return n+'/'+d;
}

UnlimitedRational* UnlimitedRational::add(UnlimitedRational* i1, UnlimitedRational* i2){
    if(i1->get_q()->get_sign()==0 || i2->get_q()->get_sign()==0){
        UnlimitedInt* a = new UnlimitedInt(0);
        UnlimitedInt* b = new UnlimitedInt(0);
        return new UnlimitedRational(a,b);
    }
    else if(i1->get_p()->get_sign()==0){
        UnlimitedInt* a = new UnlimitedInt(i2->get_p()->to_string());
        UnlimitedInt* b = new UnlimitedInt(i2->get_q()->to_string());
        UnlimitedRational* res = new UnlimitedRational(a,b);
        return res;
    }
    else if(i2->get_p()->get_sign()==0){
        UnlimitedInt* a = new UnlimitedInt(i1->get_p()->to_string());
        UnlimitedInt* b = new UnlimitedInt(i1->get_q()->to_string());
        UnlimitedRational* res = new UnlimitedRational(a,b);
        return res;
    }
    UnlimitedInt* a = new UnlimitedInt(i1->get_p()->to_string());
    UnlimitedInt* b = new UnlimitedInt(i1->get_q()->to_string());
    UnlimitedInt* c = new UnlimitedInt(i2->get_p()->to_string());
    UnlimitedInt* d = new UnlimitedInt(i2->get_q()->to_string());
    UnlimitedInt* e = UnlimitedInt::mul(a,d);
    UnlimitedInt* f = UnlimitedInt::mul(c,b);
    UnlimitedInt* num = UnlimitedInt::add(e,f);
    UnlimitedInt* den = UnlimitedInt::mul(b,d);
    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    return new UnlimitedRational(num ,den);
}

UnlimitedRational* UnlimitedRational::sub(UnlimitedRational* i1, UnlimitedRational* i2){
    if(i1->get_q()->get_sign()==0 || i2->get_q()->get_sign()==0){
        UnlimitedInt* a = new UnlimitedInt(0);
        UnlimitedInt* b = new UnlimitedInt(0);
        return new UnlimitedRational(a,b);
    }
    else if(i1->get_p()->get_sign()==0){
        UnlimitedInt* temp1 = new UnlimitedInt(-1);
        UnlimitedInt* temp2 = new UnlimitedInt(i2->get_p()->to_string());
        UnlimitedInt* temp3 = new UnlimitedInt(i2->get_q()->to_string());
        UnlimitedInt* temp = UnlimitedInt::mul(temp2,temp1);
        delete temp1;
        delete temp2;
        return new UnlimitedRational(temp , temp3);
    }
    else if(i2->get_p()->get_sign()==0) {
        UnlimitedInt* temp2 = new UnlimitedInt(i1->get_p()->to_string());
        UnlimitedInt* temp3 = new UnlimitedInt(i1->get_q()->to_string());
        UnlimitedRational* res = new UnlimitedRational(temp2 , temp3);
        return res;
    }
    
    UnlimitedInt* a = new UnlimitedInt(i1->get_p()->to_string());
    UnlimitedInt* b = new UnlimitedInt(i1->get_q()->to_string());
    UnlimitedInt* c = new UnlimitedInt(i2->get_p()->to_string());
    UnlimitedInt* d = new UnlimitedInt(i2->get_q()->to_string());
    UnlimitedInt* e = UnlimitedInt::mul(a,d);
    UnlimitedInt* f = UnlimitedInt::mul(c,b);
    UnlimitedInt* num = UnlimitedInt::sub(e,f);
    UnlimitedInt* den = UnlimitedInt::mul(b,d);
    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    return new UnlimitedRational(num ,den);
}

UnlimitedRational* UnlimitedRational::mul(UnlimitedRational* i1, UnlimitedRational* i2){
    if(i1->get_q()->get_sign()==0 || i2->get_q()->get_sign()==0){
        UnlimitedInt* a = new UnlimitedInt(0);
        UnlimitedInt* b = new UnlimitedInt(0);
        return new UnlimitedRational(a,b);
    }
    UnlimitedInt* a = new UnlimitedInt(i1->get_p()->to_string());
    UnlimitedInt* b = new UnlimitedInt(i1->get_q()->to_string());
    UnlimitedInt* c = new UnlimitedInt(i2->get_p()->to_string());
    UnlimitedInt* d = new UnlimitedInt(i2->get_q()->to_string());
    UnlimitedInt* num = UnlimitedInt::mul(a,c);
    UnlimitedInt* den = UnlimitedInt::mul(b,d);
    delete a;
    delete b;
    delete c;
    delete d;
    return new UnlimitedRational(num ,den);
}

UnlimitedRational* UnlimitedRational::div(UnlimitedRational* i1, UnlimitedRational* i2){
    if(i1->get_q()->get_sign()==0 || i2->get_q()->get_sign()==0 || i2->get_p()->get_sign()==0){
        UnlimitedInt* a = new UnlimitedInt(0);
        UnlimitedInt* b = new UnlimitedInt(0);
        return new UnlimitedRational(a,b);
    }
    UnlimitedInt* a = new UnlimitedInt(i1->get_p()->to_string());
    UnlimitedInt* b = new UnlimitedInt(i1->get_q()->to_string());
    UnlimitedInt* c = new UnlimitedInt(i2->get_p()->to_string());
    UnlimitedInt* d = new UnlimitedInt(i2->get_q()->to_string());
    UnlimitedInt* num = UnlimitedInt::mul(a,d);
    UnlimitedInt* den = UnlimitedInt::mul(b,c);
    delete a;
    delete b;
    delete c;
    delete d;
    return new UnlimitedRational(num ,den);
}