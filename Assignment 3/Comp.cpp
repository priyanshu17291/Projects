#include "Comp.h"

void Comp::createAccount(std::string id, int count)
{
    Account krishna;
    krishna.id = id;
    krishna.balance = count;
    int n = hash(id);
    int p = bankStorage2d.size();
    if (n >= bankStorage2d.size())
    {
        for (int i = 0; i < n - p + 1; i++)
        {
            bankStorage2d.push_back(std::vector<Account>{});
        }
    }
    if (v[n] == true)
    {
        bankStorage2d[n].push_back(krishna);
    }
    else
    {
        bankStorage2d[n].push_back(krishna);
        v[n] = true;
    }
    size++;
}

std::vector<int> Comp::getTopK(int k)
{
    std::vector<int> gh, gt;
    for (int i = 0; i < bankStorage2d.size(); i++)
    {
        for (int j = 0; j < bankStorage2d[i].size(); j++)
        {
            gh.push_back(bankStorage2d[i][j].balance);
        }
    }
    // std::sort(gh.begin(), gh.end());
    mergeSort(gh, 0, gh.size() - 1);
    for (int i = 0; i < k; i++)
    {
        gt.push_back(gh[gh.size() - 1 - i]);
    }
    return gt;
}

int Comp::getBalance(std::string id)
{
    if (v[hash(id)] == true)
    {
        for (int i = 0; i < bankStorage2d[hash(id)].size(); i++)
        {
            if (bankStorage2d[hash(id)][i].id == id)
            {
                return bankStorage2d[hash(id)][i].balance;
            }
        }
        return -1;
    }
    else
    {
        return -1;
    }
}

void Comp::addTransaction(std::string id, int count)
{
    if (v[hash(id)] == true)
    {
        for (int i = 0; i < bankStorage2d[hash(id)].size(); i++)
        {
            if (bankStorage2d[hash(id)][i].id == id)
            {
                bankStorage2d[hash(id)][i].balance += count;
                return;
            }
        }
        createAccount(id, count);
        return;
    }
    else
    {
        createAccount(id, count);
    }
}

bool Comp::doesExist(std::string id)
{
    if (v[hash(id)] == false)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < bankStorage2d[hash(id)].size(); i++)
        {
            if (bankStorage2d[hash(id)][i].id == id)
            {
                return true;
            }
        }
        return false;
    }
}

bool Comp::deleteAccount(std::string id)
{
    if (v[hash(id)] == false)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < bankStorage2d[hash(id)].size(); i++)
        {
            if (bankStorage2d[hash(id)][i].id == id)
            {
                bankStorage2d[hash(id)].erase(bankStorage2d[hash(id)].begin() + i);
                size--;
                return true;
            }
        }
        return false;
    }
}

int Comp::databaseSize()
{
    return size;
}

int Comp::hash(std::string id)
{
    int sum = 0;
    for (int i = 0; i < id.size(); i++)
    {
        sum += (i + 2) * (i + 2) * (id[i]);
    }
    return sum % (99991);
}

void Comp::merge(std::vector<int> &arr, int left, int middle, int right)
{
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
    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void Comp::mergeSort(std::vector<int> &arr, int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}