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
    {
        auto tuple =  myMemory.at(position);
        return std::get<0>(tuple);
    }

    /** Libere une variable **/
private:
    /** Vecteur de (IndexProcessus, Variable)  **/
    std::vector<std::tuple<int, int> > myMemory = new std::vector<std::tuple<int, int>> ();

};


#endif //ALGOREP_MEMORYPROCESS_H
