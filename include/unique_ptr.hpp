
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
private:
    T *p;
public:
    // Construit un unique_ptr nul.
    unique_ptr() {
        p = nullptr;
     }

    // Construit l'unique_ptr avec le pointeur passé.
    explicit unique_ptr(T* ptr) {
        p = ptr;
     }

    // Construit l'unique_ptr en transférant le pointeur d'un autre unique_ptr.
    unique_ptr(unique_ptr&& rhs) { 
        p = rhs.p;
        rhs.p = nullptr;
     }

    ~unique_ptr() {
        delete p;
    }

    // Assigne l'unique_ptr en transférant le pointeur d'un autre unique_ptr.
    unique_ptr& operator=(unique_ptr&& rhs) { 
        p = rhs.p;
        rhs.p = nullptr;    
        return *this;
    }

    // Accède à l'objet pointé.
    T*       operator->()       { return p; }
    T const* operator->() const { return p; }
    
    T&       operator*()       { return *p; }
    T const& operator*() const { return *p; }

    // Indique si le pointeur est non-nul.
    explicit operator bool() const { return p != nullptr; }
};
