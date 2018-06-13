
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/abstractions/les-lambdas-and-operateurs#challenge

#include <utility>

// On cherche à fusionner les 'function objects' donnés en un objet qui possède tous leurs
// opérateurs '()'. Cet objet doit également posséder les membres de ces function objects.

// Première solution, compatible avec C++11 :
// On hérite récursivement de tous les function objects, et on expose leur 'operator()'.
// On fournit également un constructeur qui permet de copie ou move les function objects.

template <class...Fs>
struct overload;

template <class F, class...Fs>
struct overload<F, Fs...> : F, overload<Fs...> {
    using F::operator();
    using overload<Fs...>::operator();

    template <class Arg, class...Args>
    overload(Arg&& arg, Args&&...args) : 
        F{ std::forward<Arg>(arg) },
        overload<Fs...>{ std::forward<Args>(args)... }
    {}
};

template <class F>
struct overload<F> : F {
    using F::operator();

    template <class Arg>
    overload(Arg&& arg) : 
        F{ std::forward<Arg>(arg) }
    {}
};

template <>
struct overload<> {};

template <class...Fs>
overload<Fs...> make_overload(Fs&&...fs) {
    return { std::forward<Fs>(fs)... };
}

// En C++17, on peut supprimer le constructeur, car les aggrégats peuvent avoir de l'héritage :
// On disposera donc d'un constructeur par défaut qui peut copier ou move tous les membres de
// notre objet.
//
// Peut peut également hériter de toutes les classes à la fois, et utiliser leur opérateur, ce
// qui permet d'éviter l'héritage récursif :

template <class...Fs>
struct overload2 : Fs... {
    using Fs::operator()...;
};

// Comme bonus, on peut utiliser un 'template deduction guide' pour pouvoir contruire l'objet
// sans indiquer ses paramètres templates, pour éviter d'utiliser la fonction make_overload :

template <class...Fs>
overload2(Fs&&...fs) -> overload2<Fs...>;

#include <fstream>
#include <tuple>

void test() {
    auto os = std::ofstream{ "logs.txt" };
    auto elements = std::tuple{ 42, "hello" };

    auto f = make_overload(
        [&os] (int i)           { os << "int = " << i; },
        [&os] (char const* str) { os << "str = " << str; }
    );
    f(std::get<0>(elements)); // int = 42
    f(std::get<1>(elements)); // str = hello

    auto f2 = overload2{
        [&os] (int i)           { os << "| int = " << i; },
        [&os] (char const* str) { os << "| str = " << str; }
    };
    f2(std::get<0>(elements)); // | int = 42
    f2(std::get<1>(elements)); // | str = hello
}
