//
// Created by Coke Lin.
//
//#define DEBUG_ENABLED
#include <iostream>
#include "Node.h"
#include "CokeSkipList.h"
#include <thread>

// 1 131.95s
#define THREAD_NUM 1
#define TEST_NUM 100000

CokeSkipList<int, std::string> skipList(15);
void insertElement(int tid) {
//    std::cout << "insertElement Thread id = " << tid << std::endl;
    int times = TEST_NUM / THREAD_NUM;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, TEST_NUM - 1);
    for (int i = 0; i < times; i++) {
        skipList.insertElement(dis(gen) % TEST_NUM, "abc");
    }
}

void getElement(int tid) {
//    std::cout << "getElement Thread id = " << tid << std::endl;
    int tmp = TEST_NUM / THREAD_NUM;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, TEST_NUM - 1);
    for (int count = 0; count < tmp; count++) {
        skipList.searchElement(dis(gen) % TEST_NUM);
    }
}

void stressTestInsert() {
    std::thread threads[THREAD_NUM];

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Ready to do stress tests..." << std::endl;
    for (int i = 0; i < THREAD_NUM; i++) {
        threads[i] = std::thread(insertElement, i);
    }
    for (int i = 0; i < THREAD_NUM; i++) {
        threads[i].join();
    }
    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Insert elapsed: " << elapsed.count() << std::endl;
}

void stressTestGet() {
    std::thread threads[THREAD_NUM];

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Ready to do stress tests..." << std::endl;
    for (int i = 0; i < THREAD_NUM; i++) {
        threads[i] = std::thread(getElement, i);
    }
    for (int i = 0; i < THREAD_NUM; i++) {
        threads[i].join();
    }
    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "get elapsed: " << elapsed.count() << std::endl;
}

int main() {
    stressTestInsert();

//    skipList.printSkipList();

//    stressTestGet();
    return 0;
}