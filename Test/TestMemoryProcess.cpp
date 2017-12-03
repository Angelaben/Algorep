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
    LOG(NAME, "Test of allocate and print memory")
    LOG(NAME, "Expected 0 0 0 0 0")
    auto res =  memory.allocateMemory(5, 0, 0);
    memory.printMemory();
    std::cout << "Position attendu 0, position obtenu " << res << std::endl;
    LOG(NAME, "Expected 0 0 0 0 0 42 42")
    res =  memory.allocateMemory(2, 0, 42);
    memory.printMemory();
    std::cout << "Position attendu 5 (fin du précédent), position obtenu " << res << std::endl;
    LOG(NAME, " Test GetValue ")
    LOG(NAME, " Expect 0 and 42 ")
    auto first = memory.getValue(0);
    auto second = memory.getValue(5);
    std::cout << " First " << first << " and second " << second << std::endl;
    MemoryProcess memory_max_Size(5, 10);
    LOG(NAME, " Test Max Size, expected 10")
    std::cout << "Got : " << memory_max_Size.getMaxSize() << std::endl;
    LOG(NAME, " Test GetCurrentSize expcted 7")
    std::cout << "Got " << memory.getCurrentSize() << std::endl;
    LOG(NAME, " Test free vector, expected 0")
    memory.freeMemory();
    std::cout << "Got : " << memory.getCurrentSize() << std::endl;

    LOG(NAME, " Test Free Variable, expected array of 42 ");
    res =  memory.allocateMemory(5, 0, 0);
    res =  memory.allocateMemory(2, 0, 42);
    memory.freeVariable(0, 0);
    memory.printMemory();
    LOG(NAME, " Test Free Variable, expected array of 42 then 5 of value 0");
    res =  memory.allocateMemory(5, 0, 0);
    memory.freeVariable(0, 2);
    memory.printMemory();
}