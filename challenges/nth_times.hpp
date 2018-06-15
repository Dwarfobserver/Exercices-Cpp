
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/abstractions/les-templates-2-2#challenges

#include <utility>

// Voici la première version, avec une simple récursion.
// Le cas d'arrêt est géré avec 'if constexpr'.

namespace detail {
    template <class F, size_t I, size_t N>
    void nth_times(F& f) {
        if constexpr (I < N) {
            f();
            nth_times<F, I + 1, N>(f);
        }
    }
}

// Une lambda est utilisé pour séparer l'argument spécifié
// par l'utilisateur (N) de celui qui devrait être déduit (F).
template <size_t N>
constexpr auto nth_times = [] (auto&& f) {
    detail::nth_times<decltype(f), 0, N>(f);
};

// Cette version utilise une fold expression.
// std::make_index_sequence<N> est utilisé pour générer N
// paramètres, pour appeler f N fois.

namespace detail {
    template <class F, size_t...Is>
    void nth_times2(F& f, std::index_sequence<Is...>) {
        // Is est cast en void pour éviter les warnings disant
        // que Is est inutilisé.
        (..., (static_cast<void>(Is), f()));
    }
}

template <size_t N>
constexpr auto nth_times2 = [] (auto&& f) {
    detail::nth_times2(f, std::make_index_sequence<N>{});
};
