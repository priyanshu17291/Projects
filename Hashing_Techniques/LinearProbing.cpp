#include "LinearProbing.h"

void LinearProbing::createAccount(std::string id, int count)
{
    Account priyanshu;
    priyanshu.id = id;
    priyanshu.balance = count;
    int n = hash(id);
    size++;
    for (int i = n; i < bankStorage1d.size(); i++)
    {
        if (v[i] == false)
        {
            bankStorage1d[i] = priyanshu;
            v[i] = true;
            return;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (v[i] == false)
        {
            bankStorage1d[i] = priyanshu;
            v[i] = true;
            return;
        }
    }
}

std::vector<int> LinearProbing::getTopK(int k)
{
    std::vector<int> gh, gt;
    for (int i = 0; i < bankStorage1d.size(); i++)
    {
        if (v[i] == true)
        {
            gh.push_back(bankStorage1d[i].balance);
        }
    }
    //std::sort(gh.begin(), gh.end());
    mergeSort(gh ,0 ,gh.size()-1);
    for (int i = 0; i < k; i++)
    {
        gt.push_back(gh[gh.size() - 1 - i]);
    }
    return gt;
}

int LinearProbing::getBalance(std::string id)
{
    int n = hash(id);
    for (int i = n; i < bankStorage1d.size(); i++)
    {
        if (id == bankStorage1d[i].id && v[i] == true)
        {
            return bankStorage1d[i].balance;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (id == bankStorage1d[i].id && v[i] == true)
        {
            return bankStorage1d[i].balance;
        }
    }
    return -1;
}

void LinearProbing::addTransaction(std::string id, int count)
{
    int n = hash(id);
    for (int i = n; i < bankStorage1d.size(); i++)
    {
        if (id == bankStorage1d[i].id && v[i] == true)
        {
            bankStorage1d[i].balance += count;
            return;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (id == bankStorage1d[i].id && v[i] == true)
        {
            bankStorage1d[i].balance += count;
            return;
        }
    }
    createAccount(id, count);
}

bool LinearProbing::doesExist(std::string id)
{
    int n = hash(id);
    for (int i = n; i < bankStorage1d.size(); i++)
    {
        if (id == bankStorage1d[i].id && v[i] == true)
        {
            return true;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (id == bankStorage1d[i].id && v[i] == true)
        {
            return true;
        }
    }
    return false;
}

bool LinearProbing::deleteAccount(std::string id)
{
    int n = hash(id);
    for (int i = n; i < bankStorage1d.size(); i++)
    {
        if (id == bankStorage1d[i].id && v[i] == true)
        {
            v[i] = false;
            size--;
            return true;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (id == bankStorage1d[i].id && v[i] == true)
        {
            v[i] = false;
            size--;
            return true;
        }
    }
    return false;
}

int LinearProbing::databaseSize()
{
    return size;
}

int LinearProbing::hash(std::string id)
{
    int sum = 0;
    for (int i = 0; i < id.size(); i++)
    {
        sum += (i + 2) * (i + 2) * (id[i]);
    }
    return sum % (99991);
}

void LinearProbing::merge(std::vector<int>& arr, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;
    std::vector<int> leftArr(n1);
    std::vector<int> rightArr(n2);
    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[middle + 1 + j];
    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void LinearProbing::mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}