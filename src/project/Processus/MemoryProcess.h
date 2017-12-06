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
    void freeMemory() { myMemory.clear(); freeArray.clear();}
    /**
     * Permet de demander l’allocation d’une variable.
     * Cette fonction doit retourner un identifiant permettant d’y
       accéder depuis n’importe quel processus.
       TODO : Gerer le cas ou on deborde de la mémoire dispo, a donner a un frere
    **/
    int allocateMemory(int identifiant, int value);
    /** Permet de demander la lecture d’une variable.
     * Cette fonction doit retourner la valeur de la variable
     *
     **/
    std::vector<int> allocateMultiMemory(int identifiant, std::vector<int> values);
    bool modifyMemory(int position,int id, int new_value);


    void freeVariable(int position, int identifiant);
    void freeMutlipleVariable(std::vector<int> vect_position, int identifiant);

    int getValue(int position) { return std::get<1>(myMemory.at(position)); }

    /** Fonction Annexe **/
    void printMemory();
    int getMaxSize() { return max_size;}
    std::vector<std::tuple<int, int>> getVector() { return myMemory;}

    int getId() { return id; }
    int getCurrentSize() ;

private:
    /** Vecteur de (IndexProcessus, Variable) **/
    std::vector<std::tuple<int, int>> myMemory = std::vector<std::tuple<int, int>>();
     // Marquer les positions free, True si free, False si non free
    std::vector<bool> freeArray = std::vector<bool>();
    int id = -1;
    int max_size = 0;
    /** Vue qu'on supprime et ajoute, on aura des décalages dans les positions renvoyés précédemment par allocate et free
     * pour l'instant les variables ne sont pas free mais juste marqué free, pas de libération de mémoire
     * Idée pour améliorer : Parcourir le tableau de free, et a partir du dernier element non free, on efface. On ne peut pas
     * supprimer ceux d'avant car les positions relatives sont encores utiles
     * Idée 2 : trouver un moyen de modifier les positions en supprimant la mémoire
     * Idée 3 : se servir de freeArray pour renvoyer les positions et allocate
     * **/
};


#endif //ALGOREP_MEMORYPROCESS_H
