
/*
    Ces exercices sont des manipulations de templates pour exécuter
    des algorithmes à la compilation.
*/

#pragma once

#include <tuple>


// Définit le type correspondant à celui de Tuple, avec la classe T
// ajoutée en dernier paramètre.
template <class T, class Tuple>
struct add_to {
    using type = void;
};

// Définit le type Tuple où la première occurence de T est supprimée.
// Si T n'est pas trouvé, alias le tuple.
template <class T, class Tuple>
struct remove_from {
    using type = void;
};

// Définit un booléen qui indique si la classe T1 est strictement
// plus petite que T2.
template <class T1, class T2>
struct is_smaller_than {
    static constexpr bool value = false;
};

// Définit le tuple trié selon le prédicat passé en paramètre : le
// booléen Predicate<T1, T2>::value indique si T1 doit être avant T2.
template <class Tuple, template <class, class> class Predicate>
struct sorted {
    using type = void;
};
