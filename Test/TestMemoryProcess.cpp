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
    std::cout << "First ID expected -1, got : " << memory.getId() << std::endl;
    std::cout << "Second ID expected 5, got : " << memory_id.getId() << std::endl;

    std::cout << " ============================= " << std::endl;
    LOG(NAME, "Test of allocate and print memory")
    LOG(NAME, "Expected id : 0, value : 0")
    auto res = memory.allocateMemory(0, 0);
    memory.printMemory();
    std::cout << "Position attendu 0, position obtenu " << res << std::endl;
    std::cout << " ============================= " << std::endl;
    LOG(NAME, "Expected 0 42")
    res = memory.allocateMemory(0, 42);
    memory.printMemory();
    std::cout << "Position attendu 1 (fin du précédent), position obtenu " << res << std::endl;
    std::cout << " ============================= " << std::endl;

    LOG(NAME, " Test GetValue ")
    LOG(NAME, " Expect 0 and 42 ")
    auto first = memory.getValue(0);
    auto second = memory.getValue(1);
    std::cout << " First " << first << " and second " << second << std::endl;
    std::cout << " ============================= " << std::endl;

    MemoryProcess memory_max_Size(5, 10);
    LOG(NAME, " Test Max Size, expected 10")
    std::cout << "Got : " << memory_max_Size.getMaxSize() << std::endl;
    std::cout << " ============================= " << std::endl;

    LOG(NAME, " Test GetCurrentSize expcted 2")
    std::cout << "Got " << memory.getCurrentSize() << std::endl;
    std::cout << " ============================= " << std::endl;

    LOG(NAME, " Test free vector, expected 0")
    memory.freeMemory();
    std::cout << "Got : " << memory.getCurrentSize() << std::endl;
    std::cout << " ============================= " << std::endl;

    LOG(NAME, " Test Free Variable, expected array 42 after deleting 0 ");
    res = memory.allocateMemory(0, 0);
    res = memory.allocateMemory(0, 42);
    memory.freeVariable(0, 0);
    memory.printMemory();
    std::cout << " ============================= " << std::endl;


    LOG(NAME, " Test Modify variables : Expected 25 42 2 3 after modyfing 0")
    res = memory.allocateMemory(0, 0);
    res = memory.allocateMemory(0, 2);
    res = memory.allocateMemory(0, 3);
    res = memory.modifyMemory(0, 0, 25);
    memory.printMemory();
    std::cout << "Got " << res << " expected True" << std::endl;
    std::cout << " ============================= " << std::endl;

    LOG(NAME, " Test Modify variables  invalid position, expect error")
    res = memory.modifyMemory(30, 0, 42);
    LOG(NAME, " Test freeMultipleVariable and allocateMultipleVariable")
    MemoryProcess memory_b;
    std::vector<int> value = {3, 10, 20};
    auto vect_position = memory_b.allocateMultiMemory(0, value);
    LOG(NAME, " Expected result : 3 10 20")
    memory_b.printMemory();
    std::cout << " ============================= " << std::endl;

    LOG(NAME, " Expected result : empty")
    memory_b.freeMutlipleVariable(vect_position, 0);
    std::cout << " Size of vector " << memory_b.getCurrentSize() << std::endl;





}