//
// Created by Coke Lin.
//
#ifndef MYSKIPLIST_COKESKIPLIST_H
#define MYSKIPLIST_COKESKIPLIST_H

#include "Node.h"
#include <memory>
#include <iostream>
#include <random>
#include <fstream>
//#include <mutex>


template<typename K, typename V>
class CokeSkipList {
public:
    CokeSkipList(int);
    ~CokeSkipList();

    // 基础功能
    Node<K, V>* createNode(K, V, int);
    int getRandomLevel();
    int insertElement(K, V); // 插入节点
    Node<K, V>* searchElement(K); // 查找节点
    bool deleteElement(K); // 删除节点

    // 额外功能
    void printSkipList();
    int size();
    void saveToDisk(std::string);
    void readFromDisk(std::string, std::string);
    void clear();

private:
    int maxLevel;
    int currLevel;
    Node<K, V>* header;
    int elementCount;
    std::ifstream myFileReader;
    std::ofstream myFileWriter;
};

template<typename K, typename V>
CokeSkipList<K, V>::CokeSkipList(int maxLevel) {
    std::cout<< "constructor CokeSkipList" << std::endl;
    this->maxLevel = maxLevel;
    this->currLevel = 0;
    this->elementCount = 0;

    K key;
    V value;
    this->header = new Node<K, V>(key, value, maxLevel);
};

template<typename K, typename V>
CokeSkipList<K, V>::~CokeSkipList(){
    std::cout<< "destructor CokeSkipList" << std::endl;
};

// 根据key value 以及节点所包括的level创建节点
template<typename K, typename V>
Node<K, V>* CokeSkipList<K, V>::createNode(K key, V value, int level) {
    auto ptr = new Node<K, V>(key, value, level);
    return ptr;
};

template<typename K, typename V>
void CokeSkipList<K, V>::printSkipList() {
    Node<K, V>* current = header;
    std::cout << "-----------[PRINT SKIPLIST]-------------------" << std::endl;
    for (int i = currLevel; i >= 0; i--) {
        current = header;
        std::cout << "level" << i << std::endl;
        while(current->nexts[i] != nullptr) {
            current = current->nexts[i];
            std::cout << "key = " << current->getKey() << " value = " << current->getValue() << "; ";
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------------------------" << std::endl;
}

template<typename K, typename V>
int CokeSkipList<K, V>::size() {
    return elementCount;
}

template<typename K, typename V>
Node<K, V>* CokeSkipList<K, V>::searchElement(K key) {
#ifdef DEBUG_ENABLED
    std::cout << "Search Element with Key = " << key << std::endl;
#endif
    Node<K, V>* current = header;
    // 从最高层往下找
    for (int i = currLevel; i >= 0; i--) {
        while (current->nexts[i] && current->nexts[i]->getKey() < key) {
            current = current->nexts[i]; // 单层单链表遍历，直到下一个数大于等于key或者遍历完毕单链表
        }
    }

    // 这时候已经到了第0层
    current = current->nexts[0];

    if (current != nullptr && current->getKey() == key) {
#ifdef DEBUG_ENABLED
        std::cout << "[SEARCH SUCCESS] SkipList finds Key = " << key << ", value = " << current->getValue() << std::endl;
#endif
        return current;
    }

    // 否则则跳表中不存在该元素
#ifdef DEBUG_ENABLED
    std::cout << "[SEARCH FAILED] SkipList can't find key = " << key << std::endl;
#endif
    return nullptr;
};

// 跳表删除节点
template<typename K, typename V>
bool CokeSkipList<K, V>::deleteElement(K key) {
    Node<K, V>* ans = searchElement(key);
    // 如果压根就没有该元素，则删除失败返回false
    if (ans == nullptr) return false;

    Node<K, V>* current = header;
    Node<K, V>* deleteNode = nullptr;

    for (int i = currLevel; i >= 0; i--) {
        while (current->nexts[i] != nullptr && current->nexts[i]->getKey() < key) {
            current = current->nexts[i];
        }
        // 如果第i层中出现了需要删除的元素，通过链表指针的改动实现删除
        if (current->nexts[i] != nullptr && current->nexts[i]->getKey() == key) {
            deleteNode = current->nexts[i];
            current->nexts[i] = current->nexts[i]->nexts[i];
        }
    }

    for (int i = currLevel; i >= 0; i--) {
        // 表示当前层没有元素了
        if (header->nexts[i] == nullptr) {
            currLevel--;
        }
    }
    // 删除节点，会打印destructor Node
    delete deleteNode;
    elementCount--;
#ifdef DEBUG_ENABLED
    std::cout << "[DELETE SUCCESS] delete key = " << key << std::endl;
#endif
    return true;
};

// 投硬币，看新加入的节点能到第几层，从而维持每一层的节点个数期望值为上一层的两倍（跳表的一个精髓）
template<typename K, typename V>
int CokeSkipList<K, V>::getRandomLevel() {
    int level = 0;

    // 这里很奇怪，如果用rand()的话会出现每次运行结果都是一样的bug，可能和rand是伪随机数有关
    std::random_device rd;
    std::mt19937 gen(rd()); // 使用 Mersenne Twister 算法的随机数引擎
    std::uniform_int_distribution<> dis(0, 1); // 创建均匀分布器

    while (dis(gen)) {
        level++;
    }

    // 不能无限增长，如果大于最大层数，则以最大层数为准
    return level > this->maxLevel ? this->maxLevel : level;
}

// 返回1 表示插入成功
// 返回0 表示元素已经存在，进行了更新
template<typename K, typename V>
int CokeSkipList<K, V>::insertElement(K key, V value) {
    // 1. 插入的元素如果存在，则进行修改
    // 2. 插入的元素如果不存在，则计算出level值，level中都加该节点
    Node<K, V>* current = header;
    std::vector<Node<K, V>*> preNode(maxLevel, nullptr);

    for (int i = currLevel; i >= 0; i--) {
        while (current->nexts[i] != nullptr && current->nexts[i]->getKey() < key) {
            current = current->nexts[i];
        }
        preNode[i] = current;
    }

    current = current->nexts[0];

    // 如果元素存在，进行更新操作
    if (current != nullptr && current->getKey() == key) {
        current->setValue(value);
#ifdef DEBUG_ENABLED
        std::cout << "[INSERT FAIL] update key = " << key << ", value = " << value << std::endl;
#endif
        return 0;
    }

    // 跳表中没有该元素
    int randomLevel = this->getRandomLevel();
//    std::cout << randomLevel << std::endl; // 打印当前节点的level

    Node<K, V>* insertNode = this->createNode(key, value, randomLevel);

    if (randomLevel > currLevel) {
        for (int i = currLevel + 1; i <= randomLevel; i++) {
            preNode[i] = header;
        }
        currLevel = randomLevel;
    }

    for (int i = 0; i <= randomLevel; i++) {
        insertNode->nexts[i] = preNode[i]->nexts[i];
        preNode[i]->nexts[i] = insertNode;
    }
#ifdef DEBUG_ENABLED
    std::cout << "[INSERT SUCCESS] inserted key = " << key << ", value = " << value << ", level = " << randomLevel << std::endl;
#endif
    elementCount++;
    return 1;
}

template<typename K, typename V>
void CokeSkipList<K, V>::saveToDisk(std::string path) {
    this->myFileWriter.open(path, std::ios::out);

    if (!this->myFileWriter.is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
        return;
    }

    Node<K, V>* current = header->nexts[0];
    while (current != nullptr) {
        this->myFileWriter << current->getKey() << ":" << current->getValue() << '\n';
        current = current->nexts[0];
    }
    this->myFileWriter.flush(); // 将缓冲区数据刷新到磁盘
    this->myFileWriter.close();
}

template<typename K, typename V>
void CokeSkipList<K, V>::readFromDisk(std::string path, std::string delimiter) {
    this->myFileReader.open(path, std::ios::in);

    if (!this->myFileReader.is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
        return;
    }

    std::string key;
    std::string value;
    std::string line;

    while (getline(this->myFileReader, line)) {
        int pos = line.find(delimiter);
        if (pos == std::string::npos) {
            std::cerr << "Error: Unable to find delimiter!" << std::endl;
            this->myFileReader.close();
            return; // 停止执行
        }
        if (pos == 0) {
//            key = "";
            std::cerr << "Error: Key cannot be null!" << std::endl;
            this->myFileReader.close();
            return;
        }
        else key = line.substr(0, pos);
        if (pos + 1 == line.length()) {
//            value = "";
            std::cerr << "Error: Value cannot be null!" << std::endl;
            this->myFileReader.close();
            return;
        }
        else value = line.substr(pos + 1, line.length() - pos - 1);

        this->insertElement(std::stoi(key), value);
    }
    this->myFileReader.close();
}

template<typename K, typename V>
void CokeSkipList<K, V>::clear() {
    Node<K, V>* current = header->nexts[0];

    // header还要用的，不能成为悬空指针
    for (int i = 0; i <= maxLevel; i++) {
        header->nexts[i] = nullptr;
    }
    while (current != nullptr) {
        Node<K, V>* next = current->nexts[0];
        delete current;
        current = next;
    }
    currLevel = 0;
    elementCount = 0;
    std::cout << "[CLEAR SUCCESS]" << std::endl;
}

#endif //MYSKIPLIST_COKESKIPLIST_H
