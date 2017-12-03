//
// Created by benjamin angelard on 03/12/2017.
//


#include<vector>
#include<tuple>
#include "macros.h"
#include "MemoryProcess.h"
#define NAME "TestMemoryProcess"
int main(void) {

    LOG("TestMemoryProcess", "Launch test of memoryProcess class")
    MemoryProcess memory;
    MemoryProcess memory_id(5);
    LOG("TestMemoryProcess", "Test of Constructor")
    std::cout << "First ID expected -1, got : "<< memory.getId() << std::endl;
    std::cout << "Second ID expected 5, got : "<< memory_id.getId() << std::endl;

   /*
    void freeMemory(NAME, )
    void resizeMemory(int size) { myMemory.resize(size); }
    int allocateMemory(int size, int identifiant, int value);

    int getValue(int position) {
        auto tuple = myMemory.at(position);
        return std::get<0>(tuple);
    }
*/

}