//
// Created by benjamin angelard on 03/12/2017.
//

#include "MemoryProcess.h"

int MemoryProcess::allocateMemory(int size, int identifiant, int value)
{
    int current_position = myMemory.size();
    /** TODO rajouter la gestion de la taille max d'un processus **/
/** Histoire de pas se prendre la tête, allouer la mémoire c'est
 * rajouter (identifiant, valeur)  autant de position que demandé
 */
    for (int i = 0; i < size; i++)
    {
        myMemory.push_back(std::make_tuple(identifiant, value));
    }
    return current_position;
}

void MemoryProcess::printMemory() {
    for (auto value : myMemory) {
        std::cout << "Identifiant " << std::get<0>(value)
                  << " valeur " << std::get<1>(value) << std::endl;
    }
}

void MemoryProcess::freeVariable(int position, int identifiants)
{
    int value_to_clear = std::get<1>(myMemory.at(position));
    while(true)
    {
        int val = std::get<1>(myMemory.at(position));
        int id = std::get<0>(myMemory.at(position));
        if (id == identifiants && val == value_to_clear)
            myMemory.erase(myMemory.begin() + position);
        else
            return;
    }
}