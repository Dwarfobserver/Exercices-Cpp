
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/performances/les-acces-memoire#challenges

#include <cstdlib>
#include <cstdint>

// On définit une fonction qui avance un pointeur à la prochaine adresse selon
// l'alignement requis.

namespace detail {
    template <unsigned int Align>
    void* alloca_aligned(void* mem) {
        constexpr uintptr_t mask = Align - 1;

        uintptr_t ptr = reinterpret_cast<uintptr_t>(mem);
        uintptr_t alignedPtr = (ptr + mask) & ~ mask;
        return reinterpret_cast<void*>(alignedPtr);
    }
}

// La macro alloca_aligned alloue plus de mémoire pour compenser l'alignement.
// On ne peut pas en faire une fonction, car la mémoire obtenue grace à 'alloca'
// serait perdue.
#define alloca_aligned(size, align) \
    ::detail::alloca_aligned<align>(alloca((size) + align - 1))

// Certains compilateurs offrent une fonction toute faite : par exemple pour GCC,
// __builtin_alloca_with_align.


// Bonus : on peut ensuite définir une macro pour allouer n'importe quel type
// avec alloca.
// Exemple : 'int* ptr = alloca_typed(int, 3);' allouera trois ints.
#define alloca_typed(type, nb) \
    static_cast<type*>(alloca_aligned(nb * sizeof(type), alignof(type)))
