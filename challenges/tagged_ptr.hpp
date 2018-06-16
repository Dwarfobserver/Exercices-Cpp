
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/introduction/les-categories-de-classes#challenge

#include <cstdint>
#include <cstddef>
#include <type_traits>

// Cette solution stocke le compteur à l'intérieur de l'alignement du pointeur : par exemple,
// un pointeur vers un int doit avoir une adresse alignée sur 4 octets, donc on dispose
// d'autant de bits pour le compteur.
// Une autre solution (moins portable) aurait été d'utiliser les 16 premiers bits du pointeurs,
// inutilisés sur les architectures actuelles 64 bits.

// Utilitaires bonus :

// is_power_of_two sert à vérifier à la compilation que l'alignement du pointeur est valable,
// c'est-à-dire une puissance de 2.

template <int I>
constexpr bool is_power_of_two = [] {
    if constexpr (I == 1) {
        return true;
    }
    else {
        constexpr int div = I / 2;
        if constexpr (div * 2 == I) {
            return is_power_of_two<div>;
        }
        else return false;
    }
}();

// offset_between est utilisé pour implémenter la covariance : le pointeur tagué est incrémenté
// de cet offset pour qu'il pointe vers la classe mère souhaitée.

template <class Base, class Derived>
constexpr int offset_between() {
    static_assert(std::is_base_of_v<Base, Derived>);
    intptr_t d = 1;
    auto ptr = reinterpret_cast<Derived*>(d);
    auto ptr2 = static_cast<Base*>(ptr);
    auto b = reinterpret_cast<intptr_t>(ptr2);
    return b - d;
}

// La classe permet de spécifier un plus grand alignement que celui par défaut pour le type T.

template <class T, size_t Align = alignof(T)>
class tagged_ptr {
    template <class T2, size_t A2>
    friend class tagged_ptr;
public:
    // Vérification de l'alignement à la compilation.
    static_assert(is_power_of_two<Align>, "Alignment is not a power of two");
    static_assert(Align >= alignof(T),    "Alignement is too small for T");
    static_assert(Align > 1,              "No place to store the counter");

    static constexpr int max_counter = Align - 1;

    // Le constructeur commun est appelé pour initialiser la valeur du pointeur.
    // Il est sélectionné par 'tag dispatch', grâce à 'common_ctor_t' (défini plus bas).
    tagged_ptr() noexcept     : tagged_ptr(common_ctor_t{}, 0) {}
    tagged_ptr(T* p) noexcept : tagged_ptr(common_ctor_t{}, reinterpret_cast<uintptr_t>(p)) {}

    tagged_ptr(tagged_ptr const&) = default;

    // La covariance est permise grâce à ce constructeur qui est à des pointeurs d'une classe
    // dérivée d'être changés en des pointeurs d'une de leur classe mère.
    // Note : le constructeur peut être plus général si on permet à l'alignement du pointeur
    // de changer.
    template <class Derived, class = std::enable_if_t<
        std::is_base_of_v<T, Derived>
    >>
    tagged_ptr(tagged_ptr<Derived, Align> p) : tagged_ptr(
        common_ctor_t{},
        p.value + offset_between<T, Derived>()) {}

    tagged_ptr& operator=(tagged_ptr const&) = default;

    // L'assignement covaraint (de derived vers base).
    template <class Derived, class = std::enable_if_t<
        std::is_base_of_v<T, Derived>
    >>
    tagged_ptr& operator=(tagged_ptr<Derived, Align> p) {
        value = p.value + offset_between<T, Derived>();
        return *this;
    }
    
    // Fonction manipulant le compteur.
    int get_counter()       const noexcept { return value & max_counter; }
    void set_counter(int counter) noexcept { value = pointer() + counter & max_counter; }
    void increment_counter()      noexcept { static_cast<void>(++value & max_counter); }

    // Fonctions permettant d'accéder au pointeur.
    T* ptr()        const noexcept { return reinterpret_cast<T*>(pointer()); }
    T* operator->() const noexcept { return ptr(); }
    T& operator&()  const noexcept { return *ptr(); }
private:
    uintptr_t value;

    struct common_ctor_t {};
    tagged_ptr(common_ctor_t, uintptr_t value) noexcept : value(value) {}

    uintptr_t pointer() const noexcept { return value & ~max_counter; }
};
