/*
    Cet exercice constiste à implémenter une slot_map, une structure de
    données qui stocke des objets en retournant une clé associée.
    Elle possède les propriétés suivantes :
     - Insertion d'un objet avec sa clé en O(1).
     - Suppression d'un objet avec sa clé en O(1).
     - Objets stockés de façon continue en mémoire (itération rapide).
*/

#pragma once

#include <todo.hpp>
#include <memory>
#include <tuple>


template <class T>
class slot_map {
public:
    // La clé utilisée.
    using id_type = int;

    // Les itérateurs utilisés.
    using iterator = T*;
    using const_iterator = T const*;

    // Réserve en mémoire la place pour stocker 'capacity' objets.
    void reserve(int capacity) { TODO(); }

    // Retourne le nombre d'objets stockés.
    int size() const { TODO(); }

    // Retourne la capacité actuelle, soit le nombre d'objets pouvant
    // être stockés avant de devoir réallouer de la mémoire.
    int capacity() const { TODO(); }
    
    // Construit un objet T dans le vecteur avec les argumetns passés
    // et retourne sa clé ainsi qu'une référence vers le nouvel objet.
    template <class...Args>
    std::pair<id_type, T&> emplace(Args&&...args) { TODO(); }

    // Supprime l'objet associé à la clé donnée.
    void erase(id_type id) { TODO(); }

    // Retourne un itérateur sur le premier élément de la collection.
    iterator       begin()       { TODO(); }
    const_iterator begin() const { TODO(); }

    // Retourne un itérateur sur le dernier élément de la collection.
    iterator       end()       { TODO(); }
    const_iterator end() const { TODO(); }

    // Accède à un élément de la collection en donnant sa clé.
    T&       operator[](id_type id)       { TODO(); }
    T const& operator[](id_type id) const { TODO(); }
};
