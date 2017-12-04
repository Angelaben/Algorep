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

bool MemoryProcess::modifyMemory(int position, int id, int new_value)
{

    if (position >= myMemory.size())
    {
        LOG("MemoryProcessError", "Can't modify variables, position out of bound");
        return false;
    }
    int value_to_clear = std::get<1>(myMemory.at(position));
    int current_position = position;
    bool modified = false;
    while(true)
    {
        int val = std::get<1>(myMemory.at(current_position));
        if (val == value_to_clear)
        {
            myMemory.at(current_position) = std::make_tuple(id, new_value);
            modified = true;
        }
        else
            return modified;
        current_position++;
    }
}

void MemoryProcess::freeMutlipleVariable(std::vector<int> vect_position, int identifiant)
{
    for (int position : vect_position)
        freeVariable(position, identifiant);
}

std::vector<int> MemoryProcess::allocateMultiMemory(std::vector<int> size, int identifiant, std::vector<int> values)
{
    /** TODO gestion erreur **/
    std::vector<int> res();
    for (int i = 0; i < values.size(); i++)
    {
        int taille = size.at(i);
        int val = values.at(i);
        auto pos = allocateMemory(taille, identifiant, val);
        res().push_back(pos);
    }
    return res;
}