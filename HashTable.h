//
// Created by 93539 on 2023/8/9.
//

#ifndef HASHTABLE_HASHTABLE_H
#define HASHTABLE_HASHTABLE_H

#include <iostream>
#include <vector>
#include <string>

namespace wyp {
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

    namespace closeHash {
        enum State {
            EMPTY,
            DELETE,
            EXIST,
        };

        template<class K, class V>
        struct HashData {
            State _state;
            std::pair<K, V> _kv;
        };

        template<class K, class V, class Hash = wyp::HashFunc<K>>
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

    namespace bucketHash {
        template<class K, class V>
        struct HashNode {
            std::pair<K, V> _kv;
            HashNode<K, V> *_next;

            HashNode(const std::pair<K, V> &kv)
                    : _kv(kv),
                      _next(nullptr) {

            }
        };

        template<class K, class V, class Hash = wyp::HashFunc<K>>
        class HashTable {
            typedef HashNode<K, V> Node;
        public:
            HashTable() {
                _table.resize(10);
            }

            ~HashTable() {
                for (size_t i = 0; i < _table.size(); ++i) {
                    Node *cur = _table[i];
                    while (cur != nullptr) {
                        Node *next = cur->_next;
                        delete cur;
                        cur = next;
                    }
                    _table[i] = nullptr;
                }
            }

            bool insert(const std::pair<K, V> &kv) {
                if (find(kv.first)) {
                    return false;
                }
                if (_table.size() == _n) { // 负载因子控制在1 否则就扩容
                    std::vector<Node *> newTable;
                    newTable.resize(__stl_next_prime(_table.size()));
                    for (size_t i = 0; i < _table.size(); ++i) {
                        Node *cur = _table[i];
                        while (cur) {
                            Node *next = cur->_next;
                            size_t index = Hash()(cur->_kv.first) % newTable.size();
                            cur->_next = newTable[index];
                            newTable[index] = cur;
                            cur = next;
                        }
                        _table[i] = nullptr;
                    }
                    _table.swap(newTable);
                }
                size_t index = Hash()(kv.first) % _table.size();
                Node *newNode = new Node(kv);
                newNode->_next = _table[index];
                _table[index] = newNode;
                ++_n;
                return true;
            }

            Node *find(const K &key) {
                size_t index = Hash()(key) % _table.size();
                Node *cur = _table[index];
                while (cur != nullptr) {
                    if (cur->_kv.first == key) {
                        return cur;
                    } else {
                        cur = cur->_next;
                    }
                }
                return nullptr;
            }

            bool erase(const K &key) {
                size_t index = Hash()(key) % _table.size();
                Node *prev = nullptr;
                Node *cur = _table[index];
                while (cur) {
                    if (cur->_kv.first == key) {
                        if (cur == _table[index]) {
                            _table[index] = cur->_next;
                        } else {
                            prev->_next = cur->_next;
                        }
                        delete cur;
                        --_n;
                        return true;
                    } else {
                        prev = cur;
                        cur = cur->_next;
                    }
                }
                return false;
            }

            inline unsigned long __stl_next_prime(unsigned long n) {
                static const int __stl_num_primes = 28;
                static const unsigned long __stl_prime_list[__stl_num_primes] =
                        {
                                53, 97, 193, 389, 769,
                                1543, 3079, 6151, 12289, 24593,
                                49157, 98317, 196613, 393241, 786433,
                                1572869, 3145739, 6291469, 12582917, 25165843,
                                50331653, 100663319, 201326611, 402653189, 805306457,
                                1610612741, 3221225473, 4294967291
                        };

                for (int i = 0; i < __stl_num_primes; ++i) {
                    if (__stl_prime_list[i] > n) {
                        return __stl_prime_list[i];
                    }
                }

                return __stl_prime_list[__stl_num_primes - 1];
            }

        private:
            std::vector<Node *> _table;
            size_t _n = 0;
        };
    }

} // wyp

#endif //HASHTABLE_HASHTABLE_H
