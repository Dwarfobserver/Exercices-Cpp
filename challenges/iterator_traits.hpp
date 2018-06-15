
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/abstractions/les-templates-2-2#challenges

#include <iterator>
#include <type_traits>

// is_iterator va couvrir les propriétés principales du concept ForwardIterator :
// https://en.cppreference.com/w/cpp/named_req/ForwardIterator

template <class T, class = void>
constexpr bool is_iterator = false;

template <class T>
constexpr bool is_iterator<T,
std::void_t<
    // Définitions de types.
    // iterator_traits retombe sur les types de la classe du même nom, et permet
    // de traiter à part les pointeurs (qui sont des itérateurs valides).
    typename std::iterator_traits<T>::iterator_category,
    typename std::iterator_traits<T>::value_type,
    typename std::iterator_traits<T>::pointer,
    typename std::iterator_traits<T>::reference,
    typename std::iterator_traits<T>::difference_type,

    // Copies et destructeur.
    decltype(T(std::declval<T const&>())),
    decltype(std::declval<T&>() = std::declval<T const&>()),
    decltype(std::declval<T&>().~T()),

    // Comparaison.
    decltype(std::declval<T const&>() == std::declval<T const&>()),
    decltype(std::declval<T const&>() != std::declval<T const&>()),

    // Incrémentation.
    decltype(  std::declval<T&>()++),
    decltype(++std::declval<T&>()),

    // Déréférencement.
    decltype(*std::declval<T&>()),
    decltype(*std::declval<T const&>())
>> = true;


// is_iterable vérifie que les fonctions std::begin(T) et std::end(T) existent, et
// qu'elles retournent un type vérifiant le trait is_iterator.
// std::begin et std::end vont vérifier que la classe possède les méthodes begin()
// et end(), et permet de traiter les tableaux natifs (comme int [3]) à part.

template <class T, class = void>
constexpr bool is_iterable = false;

template <class T>
constexpr bool is_iterable<T,
std::enable_if_t<
    is_iterator<decltype(std::begin(std::declval<T const&>()))> &&
    is_iterator<decltype(std::end  (std::declval<T const&>()))>
>> = true;
