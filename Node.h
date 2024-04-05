//
// Created by Coke Lin.
//
#ifndef MYSKIPLIST_NODE_H
#define MYSKIPLIST_NODE_H

#include <vector>
#include <iostream>

template<typename K, typename V>
class Node {
public:
//    Node(){};
    Node(K k, V v, int);
    ~Node();
    K getKey();
    V getValue();
    void setValue(V);
    std::vector<Node<K, V>*> nexts;
//    Node<K, V> **nexts;
    int nodeLevel{};
private:
    K key;
    V value;
};

template<typename K, typename V>
Node<K, V>::Node(K k, V v, int level) {
    this->key = k;
    this->value = v;
    this->nodeLevel = level;
    this->nexts.resize(level + 1, nullptr);
}

template<typename K, typename V>
Node<K, V>::~Node() {
//    std::cout << "destructor Node " << std::endl;
};

template<typename K, typename V>
K Node<K, V>::getKey() {
    return this->key;
}

template<typename K, typename V>
V Node<K, V>::getValue() {
    return this->value;
}

template<typename K, typename V>
void Node<K, V>::setValue(V v) {
    this->value = v;
}

#endif //MYSKIPLIST_NODE_H
