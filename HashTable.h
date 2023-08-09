//
// Created by 93539 on 2023/8/9.
//

#ifndef HASHTABLE_HASHTABLE_H
#define HASHTABLE_HASHTABLE_H

#include <iostream>
#include <vector>
#include <string>

namespace wyp {
    namespace closeHash {
        enum State {
            EMPTY,
            DELETE,
            EXIST,
        };

        template<class K>
        struct HashFunc {
            size_t operator()(const K &key) {
                return (size_t) key;
            }
        };

        template<>
        struct HashFunc<std::string> {
            size_t operator()(const std::string &key) {
                size_t hash = 0;
                for (auto ch: key) {
                    hash *= 131;
                    hash += ch;
                }
                return hash;
            }
        };

        template<class K, class V>
        struct HashData {
            State _state;
            std::pair<K, V> _kv;
        };

        template<class K, class V, class Hash = HashFunc<K>>
        class HashTable {
            typedef HashData<K, V> Data;
        public:
            HashTable() {
                _tables.resize(10);
            }

            bool insert(const std::pair<K, V> &kv) {
                if (find(kv.first)) {
                    return false;
                }
                if ((double) _n / _tables.size() > 0.7) { // 大于标定负载因子就需要扩容
                    HashTable<K, V> newTable;
                    newTable._tables.resize(2 * _tables.size());
                    for (auto &e: _tables) {
                        newTable.insert(e._kv);
                    }
                    _tables.swap(newTable._tables);
                }
                size_t index = Hash()(kv.first) % _tables.size();
                while (_tables[index]._state == EXIST) {
                    ++index;
                    index %= _tables.size();
                }
                _tables[index]._kv = kv;
                _tables[index]._state = EXIST;
                ++_n;
                return true;
            }

            Data *find(const K &Key) {
                size_t index = Hash()(Key) % _tables.size();
                while (_tables[index]._state != EMPTY) {
                    if (_tables[index]._state != DELETE && _tables[index]._kv.first == Key) {
                        return &_tables[index];
                    }
                    ++index;
                    index %= _tables.size();
                }
                return nullptr;
            }

            bool erase(const K &Key) {
                Data *ret = find(Key);
                if (ret != nullptr) {
                    ret->_state = DELETE;
                    --_n;
                    return true;
                }
                return false;
            }

        private:
            std::vector<Data> _tables;
            size_t _n = 0;
        };
    }
} // wyp

#endif //HASHTABLE_HASHTABLE_H
