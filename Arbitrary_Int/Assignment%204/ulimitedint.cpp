/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "ulimitedint.h"

UnlimitedInt::UnlimitedInt(){
    size = 0;
    sign = 0;
    capacity = 100;
    unlimited_int = new int[capacity];
}

UnlimitedInt::UnlimitedInt(string s){
    size = s.size();
    capacity = size+1;
    unlimited_int = new int[capacity];
    if(size==1 && s[0]=='0'){
        sign = 0;
        size = 1;
        unlimited_int[0]=0;
    }
    else if(s[0]=='-'){
        sign = -1;
        size = size - 1;
        for(int i =1 ; i<size+1 ; i++){
        unlimited_int[i-1] = s[i]-48;
        }
    }
    else{
        sign = 1;
        for(int i =0 ; i<size ; i++){
        unlimited_int[i] = s[i]-48;
        }
    }
}

UnlimitedInt::UnlimitedInt(int i){
    size = 0;
    if(i==0){
        sign=0;
        size=1;
        unlimited_int = new int[2];
        unlimited_int[0]=0;
    }
    else{
        if(i<0){
        sign = -1;
        i *= -1;
        }
        else sign = 1;
        int j = i;
        while(j>0){
            j/=10;
            size++;
        }
        capacity = 10;
        unlimited_int = new int[capacity];
        int k = size-1;
        while(i>0){
            unlimited_int[k] = i%10;
            k--;
            i /= 10;
        }
    }
}

UnlimitedInt::UnlimitedInt(int* ulimited_int, int cap, int sgn, int sz){
    size = sz;
    sign = sgn;
    capacity = cap;
    this->unlimited_int = ulimited_int;
}

UnlimitedInt::~UnlimitedInt(){
    size = 0;
    sign = 0;
    capacity = 0;
    delete[] unlimited_int;
}

int UnlimitedInt::get_size(){
    return size;
}

int* UnlimitedInt::get_array(){
    return unlimited_int;
}

int UnlimitedInt::get_sign(){
    return sign;
}

int UnlimitedInt::get_capacity(){
    return capacity;
}

// UnlimitedInt* UnlimitedInt::add(UnlimitedInt* i1, UnlimitedInt* i2);

string reverse(string s){
    string res;
    int n = s.size();
    for(int i = 0 ; i< n ; i++){
        res.push_back(s[n-i-1]);
    }
    return res;
}

int compare(UnlimitedInt* i1 , UnlimitedInt* i2){
    int* arr1 = i1->get_array();
    int* arr2 = i2->get_array();
    if(i1->get_size()>i2->get_size()) return 1;
    else if(i1->get_size()<i2->get_size()) return -1;
    else{
        int m = i1->get_size();
        for(int i = 0 ; i < m ; i++){
            if(arr1[i] > arr2[i]) return 1;
            else if(arr1[i] < arr2[i]){
                return -1;
            }
        }
        return 0;
    }
}

UnlimitedInt* UnlimitedInt::sub(UnlimitedInt* i1, UnlimitedInt* i2){
    if(i2->get_sign()==0){
        UnlimitedInt* res = new UnlimitedInt(i1->to_string());
        return res;
    }
    if(i1->get_sign()==0){
        string str = i2->to_string();
        str = reverse(str);
        if(str[str.size()-1]=='-') str.pop_back();
        else str.push_back('-');
        str = reverse(str);
        UnlimitedInt* res = new UnlimitedInt(str);
        return res;
    }
    if(i1->get_sign()==1 && i2->get_sign()==1){
        int m = i1->get_size();
        int n = i2->get_size();
        int sign = 1;
        string str;
        if(compare(i1,i2)==-1){
            string s = sub(i2,i1)->to_string();
            s = reverse(s);
            s.push_back('-');
            s = reverse(s);
            UnlimitedInt* res = new UnlimitedInt(s);
            return res;
        }
        int carry = 0;
        string a = i2->to_string();
        a = reverse(a);
        for(int i =0 ; i<m-n ; i++) a.push_back('0');
        a = reverse(a);
        UnlimitedInt* temp = new UnlimitedInt(a);
        int* arr1 = i1->get_array();
        int* arr2 = temp->get_array();
        for(int i = 0 ; i < m ; i++){
            int a = arr1[m-1-i] - carry;
            carry = 0;
            int b = arr2[m-1-i];
            if(a < b){
                a += 10;
                str.push_back(a-b+48);
                carry = 1;
            }
            else str.push_back(a-b+48);
        }
        delete temp;
        while(str[str.size()-1]=='0' && str.size() !=1){
            str.pop_back();
        }
        if(sign == -1) str.push_back('-');
        str = reverse(str);
        UnlimitedInt* res = new UnlimitedInt(str);
        return res;
    }
    else if(i1->get_sign()== -1 && i2->get_sign()==-1){
        string str1 = i1->to_string();
        string str2 = i2->to_string();
        str1 = reverse(str1);
        str2 = reverse(str2);
        str1.pop_back();
        str2.pop_back();
        str1 = reverse(str1);
        str2 = reverse(str2);
        UnlimitedInt* res1 = new UnlimitedInt(str1);
        UnlimitedInt* res2 = new UnlimitedInt(str2);
        return sub(res2,res1);
    }
    else {
        string str = i2->to_string();
        str = reverse(str);
        if(str[str.size()-1]=='-') str.pop_back();
        else str.push_back('-');
        str = reverse(str);
        UnlimitedInt* res = new UnlimitedInt(str);
        return add(i1,res);
    }
}

UnlimitedInt* UnlimitedInt::add(UnlimitedInt* i1, UnlimitedInt* i2){
    int m = i1->get_size();
    int n = i2->get_size();
    string str;
    if(i1->get_sign()== 0){
        UnlimitedInt* res = new UnlimitedInt(i2->to_string());
        return res;
    }
    if(i2->get_sign()== 0){
        UnlimitedInt* res = new UnlimitedInt(i1->to_string());
        return res;
    }
    if(i1->get_sign() == i2->get_sign()){
        if(m > n){
            int* arr1 = i1->get_array();
            int* arr2 = i2->get_array();
            int carry = 0;
            for(int i = 0 ; i<n ; i++){
                int sum = arr1[m-i-1]+arr2[n-i-1];
                str.push_back(((carry+sum)%10)+48);
                carry = (carry+sum)/10;
            }
            for(int i = n ; i<m ; i++){
                int sum = arr1[m-i-1]+carry;
                str.push_back((sum%10)+48);
                carry = sum/10;
            }
            if(carry != 0) str.push_back(carry+48);
            if(i1->get_sign()==-1) str.push_back('-');
            str = reverse(str);
        }
        else{
            int* arr1 = i1->get_array();
            int* arr2 = i2->get_array();
            int carry = 0;
            for(int i = 0 ; i<m ; i++){
                int sum = arr1[m-i-1]+arr2[n-i-1];
                str.push_back(((carry+sum)%10)+48);
                carry = (carry+sum)/10;
            }
            for(int i = m ; i<n ; i++){
                int sum = arr2[n-i-1]+carry;
                str.push_back((sum%10)+48);
                carry = sum/10;
            }
            if(carry != 0) str.push_back(carry+48);
            if(i1->get_sign()== -1) str.push_back('-');
            str = reverse(str);
        }
    }
    else{
        if(i1->get_sign()==-1){
            string str1 = i1->to_string();
            str1 = reverse(str1);
            str1.pop_back();
            str1 = reverse(str1);
            UnlimitedInt* res1 = new UnlimitedInt(str1);
            return sub(i2,res1);
        }
        else{
            string str1 = i2->to_string();
            str1 = reverse(str1);
            str1.pop_back();
            str1 = reverse(str1);
            UnlimitedInt* res1 = new UnlimitedInt(str1);
            return sub(i1,res1);
        }
    }
    UnlimitedInt* res = new UnlimitedInt(str);
    return res;
}

UnlimitedInt* UnlimitedInt::mul(UnlimitedInt* i1, UnlimitedInt* i2){
    if(i1->get_sign()==0 || i2->get_sign()==0){
        UnlimitedInt* res = new UnlimitedInt(0);
        return res;
    }
    else{
        string str;
        if (i1->get_sign()!=i2->get_sign()) str.push_back('-');
        int* arr1 = i1->get_array();
        int* arr2 = i2->get_array();
        int m=i1->get_size();
        int n=i2->get_size();
        int result[m + n] = {0};
        for (int i = m - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                int mul = arr1[i] * arr2[j];
                int sum = mul + result[i + j + 1];
                result[i + j + 1] = sum % 10;
                result[i + j] += sum / 10;
            }
        }
        int a=0;
        while(result[a]==0) a++;
        for(int i=a ; i<m+n ;i++){
            str.push_back(result[i]+48);
        }
        UnlimitedInt* res = new UnlimitedInt(str);
        return res;
    }
}

UnlimitedInt* UnlimitedInt::div(UnlimitedInt* i1, UnlimitedInt* i2){
    if(i2->get_sign()== 0){
        return nullptr;
    }
    else if(i1->get_sign()==0){
        UnlimitedInt* res = new UnlimitedInt(0);
        return res;
    }
    else if(i2->get_size()==1 && i2->get_array()[0]==1){
        if(i2->get_sign()==1){
            string str = i1->to_string();
            UnlimitedInt* res = new UnlimitedInt(str);
            return res;
        }
        else{
            UnlimitedInt* res = new UnlimitedInt(-1);
            return mul(i1,res);
        }
    }
    else if(compare(i1,i2)==-1){
        if(i1->get_sign()==i2->get_sign()){
            UnlimitedInt* res = new UnlimitedInt(0);
            return res;
        }
        else{
            return new UnlimitedInt(-1);
        }
    }
    else if(compare(i1,i2)==0){
        if(i1->get_sign()==i2->get_sign()){
            UnlimitedInt* res = new UnlimitedInt(1);
            return res;
        }
        else{
            return new UnlimitedInt(-1);
        }
    }
    else{
        string str1 = i2->to_string();
        str1 = reverse(str1);
        if(str1[str1.size()-1] == '-') str1.pop_back();
        str1 = reverse(str1);
        UnlimitedInt* res1 = new UnlimitedInt(str1);
        int* arr1 = i1->get_array();
        int size1 = i1->get_size();
        int size2 = i2->get_size();
        string ans;
        string temp;
        for(int i =0 ; i< size2-1 ;i++) temp.push_back(arr1[i]+48);
        for(int j = size2-1 ; j<size1 ; j++){
            temp.push_back(arr1[j]+48);
            int q = 0;
            UnlimitedInt* tem = new UnlimitedInt(temp);
            while(sub(tem,res1)->get_sign()>=0 ){
                tem = sub(tem,res1);
                q++;
            }
            ans.push_back(q+48);
            temp = tem->to_string();
            if(temp == "0") temp.pop_back();
            delete tem;
        }
        ans = reverse(ans);
        delete res1;
        while(ans[ans.size()-1]=='0' && ans.size() !=1){
            ans.pop_back();
        }
        ans = reverse(ans);
        UnlimitedInt* an = new UnlimitedInt(ans);
        if(i1->get_sign() != i2->get_sign()){
            UnlimitedInt* temp = new UnlimitedInt(-1);
            UnlimitedInt* an2 = sub(an,temp);
            an2 = mul(an2 ,temp);
            return an2;
        }
        return an;
    }
}

UnlimitedInt* UnlimitedInt::mod(UnlimitedInt* i1, UnlimitedInt* i2){
    UnlimitedInt* temp = div(i1,i2);
    UnlimitedInt* temp1 = mul(temp,i2);
    UnlimitedInt* temp2 = sub(i1,temp1);
    return temp2;
}

string UnlimitedInt::to_string(){
    string str;
    if(sign == -1) str.push_back('-');
    for(int i=0 ; i<size ; i++){
        str.push_back(unlimited_int[i]+48);
    }
    return str;
}