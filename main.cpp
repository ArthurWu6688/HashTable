#include "HashTable.h"

void test_closeHash() {
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

void test_closeHash1() {
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

int main() {
    test_closeHash();
    test_closeHash1();

    return 0;
}
