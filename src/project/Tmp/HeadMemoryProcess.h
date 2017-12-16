//
// Created by benjamin angelard on 06/12/2017.
//

#ifndef ALGOREP_HEADMEMORYPROCESS_H
#define ALGOREP_HEADMEMORYPROCESS_H
#include "MemoryProcess.h"



class HeadMemoryProcess : public MemoryProcess {
public:
    HeadMemoryProcess(int i) { rank = i;}
    HeadMemoryProcess() { rank = -1;}
    int getCurrentSize();
    void giveMe_A_Child(MemoryProcess fils) { listOfSons.push_back(fils);}
    std::tuple<int, int> allocateFirstFit(int identifiant, int value);
private :
    std::vector<MemoryProcess> listOfSons =  std::vector<MemoryProcess>();
    int rank;
};


#endif //ALGOREP_HEADMEMORYPROCESS_H
