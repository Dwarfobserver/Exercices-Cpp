
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
    // Construit un unique_ptr nul.
    unique_ptr() { TODO(); }

    // Construit l'unique_ptr avec le pointeur passé.
    explicit unique_ptr(T* ptr) { TODO(); }

    // Construit l'unique_ptr en transférant le pointeur d'un autre unique_ptr.
    unique_ptr(unique_ptr&& rhs) { TODO(); }

    // Assigne l'unique_ptr en transférant le pointeur d'un autre unique_ptr.
    unique_ptr& operator=(unique_ptr&& rhs) { TODO(); }

    // Accède à l'objet pointé.
    T*       operator->()       { TODO(); }
    T const* operator->() const { TODO(); }
    
    T&       operator*()       { TODO(); }
    T const& operator*() const { TODO(); }

    // Indique si le pointeur pointe vers un objet.
    explicit operator bool() const { TODO(); }
};
