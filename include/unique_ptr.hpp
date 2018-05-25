
/*      (facile)
    Cet exercice consiste à implémenter unique_ptr. Cette classe
    doit gérer le pointeur passé automatiquement en le détruisant
    (avec delete) dans son propre destructeur.
    Cette classe peut être move, pour transférer son pointeur à
    un autre unique_ptr.
*/

#pragma once

#include <todo.hpp>


template <class T>
class unique_ptr {
public:
    unique_ptr() { TODO(); }

    explicit unique_ptr(T* ptr) { TODO(); }

    unique_ptr(unique_ptr&& rhs) { TODO(); }

    unique_ptr& operator=(unique_ptr&& rhs) { TODO(); }

    T*       operator->()       { TODO(); }
    T const* operator->() const { TODO(); }
    
    T&       operator*()       { TODO(); }
    T const& operator*() const { TODO(); }

    explicit operator bool() const { TODO(); }
};
