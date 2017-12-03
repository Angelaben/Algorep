//
// Created by benjamin angelard on 03/12/2017.
//

#ifndef ALGOREP_MEMORYPROCESS_H
#define ALGOREP_MEMORYPROCESS_H

#include<vector>
#include<tuple>

class MemoryProcess {
public :
    MemoryProcess(){}
    MemoryProcess(int own_id) { id = own_id;}
    /** Vide la memoire du processus **/
    void freeMemory() { myMemory.clear(); }
    /** Modifie la taille de la mémoire du processus en copiant la mémoire **/
    void resizeMemory(int size) { myMemory.resize(size); }
    /**
     * Permet de demander l’allocation d’une variable.
     * Cette fonction doit retourner un identifiant permettant d’y
       accéder depuis n’importe quel processus.
    **/
    int allocateMemory(int size, int identifiant, int value);
    /** Permet de demander la lecture d’une variable.
     * Cette fonction doit retourner la valeur de la variable
     *
     **/
    int getValue(int position)
    {/*
        auto tuple =  myMemory.at(position);
        return std::get<0>(tuple);
        */
    }

    std::vector<std::tuple<int, int>> getVector() { return myMemory;}
    int getId() { return id; }
    /** Libere une variable **/
private:
    /** Vecteur de (IndexProcessus, Variable)  **/
    std::vector<std::tuple<int, int>> myMemory = std::vector<std::tuple<int, int>>();
    int id = -1;
};


#endif //ALGOREP_MEMORYPROCESS_H
