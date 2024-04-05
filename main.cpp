#include <iostream>
#include "CokeSkipList.h"
#include "Node.h"
#include <string>

#define SAVE_PATH "../persistence_data/savedata1.txt"

int main() {

//    Node<int, int> node(1, 1, 1);
    CokeSkipList<int, std::string> skipList(5);
    skipList.insertElement(1, "牢大");
    skipList.insertElement(2, "老二");
    skipList.insertElement(3, "老三");
    skipList.insertElement(4, "老四");
    skipList.printSkipList();

    skipList.searchElement(4);
    skipList.searchElement(5);
    skipList.deleteElement(4);
    skipList.deleteElement(5);
//    skipList.clear();

    skipList.readFromDisk(SAVE_PATH, ":");
    skipList.printSkipList();

//    skipList.saveToDisk(SAVE_PATH);

    return 0;
}
