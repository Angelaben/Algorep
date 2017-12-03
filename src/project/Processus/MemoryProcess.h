//
// Created by benjamin angelard on 03/12/2017.
//

#ifndef ALGOREP_MEMORYPROCESS_H
#define ALGOREP_MEMORYPROCESS_H

#include<vector>
#include<tuple>
#include "macros.h"
class MemoryProcess {
public :
    /** Constructeur **/
    MemoryProcess(){}
    MemoryProcess(int own_id) { id = own_id;}
    MemoryProcess(int own_id, int max) { id = own_id; max_size = max; }
    /** Mandatory **/
    /** Vide la memoire du processus **/
    void freeMemory() { myMemory.clear(); }
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

    /** Parcours le vecteur et supprime tous les doublons identifiants - value **/
    void freeVariable(int position, int identifiant);
    int getValue(int position) { return std::get<1>(myMemory.at(position)); }

    /** Fonction Annexe **/
    void printMemory();
    int getMaxSize() { return max_size;}
    std::vector<std::tuple<int, int>> getVector() { return myMemory;}
    int getId() { return id; }
    int getCurrentSize() { return myMemory.size();}

    /** Libere une variable **/
private:
    /** Vecteur de (IndexProcessus, Variable)  **/
    std::vector<std::tuple<int, int>> myMemory = std::vector<std::tuple<int, int>>();
    int id = -1;
    // TODO
    int max_size = 0;
};


#endif //ALGOREP_MEMORYPROCESS_H
