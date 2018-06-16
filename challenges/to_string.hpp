
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/abstractions/les-templates-1-2#challenge

#include <sstream>
#include <tuple>

// On va utiliser un ostringstream pour sérialiser les objets.
// On traitera les cas où le type :
//  - est directement sérialisable.
//  - est itérable.
//  - est une paire.
//  - est un tuple.


// On détecte si un type est streamable avec l'expression 'stream << T'.
template <class T, class = void>
constexpr bool is_streamable = false;

template <class T>
constexpr bool is_streamable<T, std::void_t<decltype(
    std::declval<std::ostringstream&>() << std::declval<T const&>()
)>> = true;

// On détecte si un type est iterable avec les expressions T.begin() et T.end().
// Cette détection peut être affinée si on utilise std::begin et std::end à
// la place.
template <class T, class = void>
constexpr bool is_iterable = false;

template <class T>
constexpr bool is_iterable<T, std::void_t<decltype(
    (std::declval<T const&>().begin(), std::declval<T const&>().end())
)>> = true;

// namespace dédié à l'implémentation de la fonction.
namespace detail {

    // Fonction pour le cas général.
    template <class T>
    void to_string(std::ostringstream& os, T const& data);
    
    // Fonction traitant les tuples.
    template <class...Ts>
    void to_string(std::ostringstream& os, std::tuple<Ts...> const& tuple);

    // Fonction traitant les paires.
    template <class T1, class T2>
    void to_string(std::ostringstream& os, std::pair<T1, T2> const& pair) {
        os << "{ ";
        to_string(os, pair.first);
        os << ", ";
        to_string(os, pair.second);
        os << " }";
    }

    // Implémentation pour les tuples (expliquée dans le chapitres des templates 2/2).
    template <size_t...Is, class...Ts>
    void to_string(
        std::ostringstream& os,
        std::tuple<Ts...> const& tuple,
        std::index_sequence<Is...>)
    {
        (..., (os << ", ", to_string(os, std::get<Is + 1>(tuple))));
    }
    template <class...Ts>
    void to_string(std::ostringstream& os, std::tuple<Ts...> const& tuple) {
        os << "{ ";
        if constexpr (sizeof...(Ts) > 0) {
            to_string(os, std::get<0>(tuple));
            using seq = std::make_index_sequence<sizeof...(Ts) - 1>;
            to_string(os, tuple, seq{});
        }
        os << " }";
    }

    // Implémentation du cas général.
    template <class T>
    void to_string(std::ostringstream& os, T const& data) {

        static_assert(is_streamable<T> || is_iterable<T>,
            "T must be streamable or iterable");

        // Cas lorsque T est streamable.
        if constexpr (is_streamable<T>) {
            os << data;
        }
        // Cas lorsque T est itérable.
        else {
            os << "[ ";
            auto it = data.begin();
            auto end = data.end();
            if (it != end) {
                to_string(os, *it);
                ++it;
                while (it != end) {
                    os << ", ";
                    to_string(os, *it);
                    ++it;
                }
            }
            os << " ]";
        }
    }
}

// to_string utilise un ostringstream pour y mettre tous les éléments de 'data'.
template <class T>
std::string to_string(T const& data) {
    std::ostringstream os;
    detail::to_string(os, data);
    return os.str();
}
