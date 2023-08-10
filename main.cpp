#include "HashTable.h"

__attribute__((unused)) void test_closeHash() {
    wyp::closeHash::HashTable<int, int> mytable;
    int a[] = {18, 8, 7, 27, 57, 3, 38, 18};
    for (auto &e: a) {
        mytable.insert(std::make_pair(e, e));
    }
    mytable.insert(std::make_pair(17, 17));
    mytable.insert(std::make_pair(5, 5));

    std::cout << mytable.find(7) << std::endl;
    std::cout << mytable.find(8) << std::endl;

    mytable.erase(7);
    std::cout << mytable.find(7) << std::endl;
    std::cout << mytable.find(8) << std::endl;
}

__attribute__((unused)) void test_closeHash1() {
    std::string arr[] = {"苹果", "西瓜", "香蕉", "草莓",
                         "苹果", "西瓜", "苹果", "苹果",
                         "西瓜", "苹果", "香蕉", "苹果",
                         "香蕉"};
    wyp::closeHash::HashTable<std::string, int> mytable;
    for (auto &e: arr) {
        if (wyp::closeHash::HashData<std::string, int> *ret = mytable.find(e)) {
            ret->_kv.second++;
        } else {
            mytable.insert(std::make_pair(e, 1));
        }
    }
}

void test_bucketHash() {
    wyp::bucketHash::HashTable<int, int> mytable;
    int a[] = {18, 8, 7, 27, 57, 3, 38, 18, 17, 88, 38, 28};
    for (auto e: a) {
        mytable.insert(std::make_pair(e, e));
    }
    mytable.insert(std::make_pair(5, 5));
    mytable.erase(17);
    mytable.erase(57);
}

void test_bucketHash2()
{
    std::string arr[] = { "苹果", "西瓜", "香蕉", "草莓", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };

    //HashTable<string, int, HashFuncString> countHT;
    wyp::bucketHash::HashTable<std::string, int> countHT;
    for (auto& e : arr)
    {
        auto ret = countHT.find(e);
        if (ret)
        {
            ret->_kv.second++;
        }
        else
        {
            countHT.insert(make_pair(e, 1));
        }
    }
}

int main() {
    test_closeHash();
    test_closeHash1();

    test_bucketHash();
    test_bucketHash2();

    return 0;
}
