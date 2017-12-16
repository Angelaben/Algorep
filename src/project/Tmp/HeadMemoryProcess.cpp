//
// Created by benjamin angelard on 06/12/2017.
//

#include "HeadMemoryProcess.h"
int HeadMemoryProcess::getCurrentSize() {
    int size = 0;
    for (auto memory : listOfSons)
    {
        size += memory.getCurrentSize();

    }
    return size;
}

std::tuple<int, int> HeadMemoryProcess::allocateFirstFit(int identifiant, int value)
{
    for (auto Node : listOfSons)
    {
        if (Node.getAvailableMemory() >= 1)
        {
            return Node.allocateMemory(identifiant, value);
        }
    }
    /** Gerer le cas Head n'a pas la place **/
    return std::make_tuple<int, int> (-1, -1);
}