#include "CubicProbing.h"

void CubicProbing::createAccount(std::string id, int count)
{
    Account krishna;
    krishna.id = id;
    krishna.balance = count;
    int n = hash(id);
    int j = 0;
    for (int i = n + j * j*j; j < bankStorage1d.size(); j++, i = n + j * j)
    {
        if (i >= bankStorage1d.size())
        {
            i %= bankStorage1d.size();
        }
        if (!v[i])
        {
            bankStorage1d[i] = krishna;
            v[i] = true;
            size++;
            return;
        }
    }
}

std::vector<int> CubicProbing::getTopK(int k)
{
    std::vector<int> gh, gt;
    for (int i = 0; i < bankStorage1d.size(); i++)
    {
        if (v[i])
        {
            gh.push_back(bankStorage1d[i].balance);
        }
    }
    //std::sort(gh.begin(), gh.end(), std::greater<int>());
    mergeSort(gh ,0 ,gh.size()-1);
    for (int i = 0; i < k && i < gh.size(); i++)
    {
        gt.push_back(gh[gh.size()-1-i]);
    }
    return gt;
}

int CubicProbing::getBalance(std::string id)
{
    int n = hash(id);
    int j = 0;
    int p = 0;
    for (int i = n + j * j*j; j < bankStorage1d.size(); j++, i = n + j * j)
    {
        if (i >= bankStorage1d.size())
        {
            i %= bankStorage1d.size();
        }
        if (id == bankStorage1d[i].id && v[i])
        {
            return bankStorage1d[i].balance;
        }
        p = i;
    }
    return -1;
}

void CubicProbing::addTransaction(std::string id, int count)
{
    int n = hash(id);
    int j = 0, p = 0;
    for (int i = n + j * j*j; j < bankStorage1d.size(); j++, i = n + j * j)
    {
        if (i >= bankStorage1d.size())
        {
            i %= bankStorage1d.size();
        }
        if (id == bankStorage1d[i].id && v[i])
        {
            bankStorage1d[i].balance += count;
            return;
        }
        p = i;
    }
    createAccount(id, count);
}

bool CubicProbing::doesExist(std::string id)
{
    int n = hash(id);
    int j = 0, p = 0;
    for (int i = n + j * j*j; j < bankStorage1d.size(); j++, i = n + j * j)
    {
        if (i >= bankStorage1d.size())
        {
            i %= bankStorage1d.size();
        }
        if (id == bankStorage1d[i].id && v[i])
        {
            return true;
        }
        p = i;
    }
    return false;
}

bool CubicProbing::deleteAccount(std::string id)
{
    int n = hash(id);
    int p = 0, j = 0;
    for (int i = n + j * j*j; j < bankStorage1d.size(); j++, i = n + j * j)
    {
        if (i >= bankStorage1d.size())
        {
            i %= bankStorage1d.size();
        }
        if (id == bankStorage1d[i].id && v[i])
        {
            v[i] = false;
            size--;
            return true;
        }
        p = i;
    }
    return false;
}

int CubicProbing::databaseSize()
{
    return size;
}

int CubicProbing::hash(std::string id)
{
    int sum = 0;
    for (int i = 0; i < id.size(); i++)
    {
        sum += (i + 2) * (i + 2) * (id[i]);
    }
    return sum % (99991);
}

void CubicProbing::merge(std::vector<int>& arr, int left, int middle, int right) {
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

void CubicProbing::mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}