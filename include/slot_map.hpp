
#pragma once

#include <todo.hpp>
#include <memory>
#include <tuple>


// Le but de l'exercice est d'implémenter une slot map : une structure qui stocke
// des objets et donne une clé pur les retrouver.
// Plusieurs approches existent, le but étant d'obtenir les propriétés suivantes :
//   - Insertion d'un objet avec sa clé en O(1)
//   - Suppression d'un objet avec sa clé en O(1)
//   - Objets stockés de façon continue en mémoire (itération rapide)
//
template <class T>
class slot_map {
public:
    using id_type = int;
    using iterator = T*;
    using const_iterator = T const*;

    void reserve(int capacity) { TODO(); }

    int size() const noexcept { TODO(); }

    int capacity() const noexcept { TODO(); }
    
    template <class...Args>
    [[nodiscard]]
    std::pair<id_type, T&> emplace(Args&&...args) { TODO(); }

    void erase(id_type id) { TODO(); }

    iterator       begin()       noexcept { TODO(); }
    const_iterator begin() const noexcept { TODO(); }

    iterator       end()       noexcept { TODO(); }
    const_iterator end() const noexcept { TODO(); }

    T&       operator[](id_type id)       noexcept { TODO(); }
    T const& operator[](id_type id) const noexcept { TODO(); }
};
