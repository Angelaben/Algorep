//
// Created by benjamin angelard on 03/12/2017.
//

#ifndef ALGOREP_MEMORYPROCESS_H
#define ALGOREP_MEMORYPROCESS_H


#include "macros.h"
class MemoryProcess {
public :
    /** Constructeur **/
    MemoryProcess(){}
    MemoryProcess(int own_id) { id = own_id;}
    MemoryProcess(int own_id, int max) { id = own_id; max_size = max; }
    /** Mandatory **/
    /** Vide la memoire du processus **/
    void freeMemory() { myMemory.clear(); freeArray.clear();}
    /**
     * Permet de demander l’allocation d’une variable.
     * Cette fonction doit retourner un identifiant permettant d’y
       accéder depuis n’importe quel processus.
       TODO : Gerer le cas ou on deborde de la mémoire dispo, a donner a un frere
    **/

    /** Renvoie un identifiant tuple sous forme <Id Possesseur, Position dans le vecteur > **/
    std::tuple<int, int> allocateMemory(int identifiant, int value);
    /** Permet de demander la lecture d’une variable.
     * Cette fonction doit retourner la valeur de la variable
     *
     **/
    std::vector<std::tuple<int,int>> allocateMultiMemory(int identifiant, std::vector<int> values);
    /** Remplacer la variable a la position P par la nouvelle value
     * TODO : Pas de gestion d'erreur pour l'instant
     * @param position
     * @param id
     * @param new_value
     */
    bool modifyMemory(int position,int id, int new_value);

    /** Explicite **/
    void freeVariable(int position, int identifiant);
    void freeMutlipleVariable(std::vector<int> vect_position, int identifiant);
    int getValue(int position) { return std::get<1>(myMemory.at(position)); }

    /** Fonction Annexe **/
    void printMemory();
    int getMaxSize() { return max_size;}
    std::vector<std::tuple<int, int>> getVector() { return myMemory;}
    int getAvailableMemory() { return max_size - getCurrentSize(); }
    int getId() { return id; }
    int getCurrentSize() ;

private:
    /** Vecteur de (IndexProcessus, Variable) **/
    std::vector<std::tuple<int, int>> myMemory = std::vector<std::tuple<int, int>>();
     // Marquer les positions free, True si free, False si non free
    std::vector<bool> freeArray = std::vector<bool>();
    int id = -1;
    int max_size = 0;

};


#endif //ALGOREP_MEMORYPROCESS_H
