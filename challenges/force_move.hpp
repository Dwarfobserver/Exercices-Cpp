
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/abstractions/les-move-semantics#challenges

#include <type_traits>


// On souhaite obtenir une erreur de compilation si le type est const :
// En effet, on veut s'assurer qu'il pourra être move (et donc modifié).

// On utilise une forwarding reference (T&&) pour récupérer n'importe quelle
// expression. On cast la référence en move reference, c'est-à-dire
// std::remove_reference_t<T>&&.

// On peut afficher une erreur à la compilation :

template <class T>
std::remove_reference_t<T>&& force_move(T&& t) {
    static_assert(!std::is_const_v<T>, "The reference musn't be const");
    return static_cast<std::remove_reference_t<T>&&>(t);
}

// Sinon, on peut supprimer la fonction dans le cas où le type est const :

template <class T>
std::remove_reference_t<T>&& force_move2(T&& t) {
    return static_cast<std::remove_reference_t<T>&&>(t);
}

template <class T>
T&& force_move2(T const& t) = delete;

