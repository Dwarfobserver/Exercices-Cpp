
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/performances/les-acces-memoire#challenges

#include <vector>
#include <stdexcept>

// Pour gagner en flexibilité, on sépare la classe respectant le concept d'allocateur
// de la ressource possédant la mémoire à allouer. Cela permet d'avoir plusieurs
// allocateurs utilisant la même ressource.

template <class T, class Resource>
class block_allocator {
public:
    using value_type = T;

    // L'allocateur possède une référence vers les blocs.
    // On vérifie à la compilation que les blocs sont assez grands pour le type
    // T de l'allocateur.
    block_allocator(Resource& r) noexcept :
            resource(r)
    {
        static_assert(Resource::template can_be_contained<T>);
    }

    // L'allocateur peut en créer un autre qui allouera des objets différents.
    template <class T2>
    block_allocator(block_allocator<T2, Resource> const& a) noexcept :
            resource(a.resource)
    {
        static_assert(Resource::template can_be_contained<T>);
    }

    // La fonction doit respecter la signature demandée. Cependant, l'allocateur
    // par blocs ne peut pas fournir plusieurs blocs continus en mémoire.
    T* allocate(int nb) {
        if (nb != 1) throw std::invalid_argument
            {"The block allocator cannot allocate multiple elements at a time"};
        
        return static_cast<T*>(resource.allocate());
    }

    void deallocate(T* ptr, int) noexcept {
        resource.deallocate(ptr);
    }
    
    Resource& resource;
};

// Les comparaisons entre allocateurs font partie des fonctions requises.

template <class T1, class T2, class R1, class R2>
bool operator==(block_allocator<T1, R1> const& rhs, block_allocator<T2, R2> const& lhs) noexcept {
    return false;
}
template <class T1, class T2, class R>
bool operator==(block_allocator<T1, R> const& rhs, block_allocator<T2, R> const& lhs) noexcept {
    return &rhs.resource == &lhs.resource;
}
template <class T1, class T2, class R1, class R2>
bool operator!=(block_allocator<T1, R1> const& rhs, block_allocator<T2, R2> const& lhs) noexcept {
    return !(rhs == lhs);
}

// Le type du bloc utilisé pour une taille donnée.
namespace detail {
    template <int Size>
    struct block_type {

        // La taille du bloc est arrondi au prochain multiple de 8 à cause de l'alignement.
        static constexpr int size = ((Size + 7) / 8) * 8;

        union {
            std::aligned_storage_t<size, 8> storage;
            block_type* next;
        };
    };
}

// La ressource possédant les blocs ne fixe pas un type particulier à utiliser.
// Cela permet d'utiliser la même pour allouer plusieurs types.

template <int Size, class Allocator = std::allocator<detail::block_type<Size>>>
class block_resource {
public:
    using block_type = detail::block_type<Size>;

    static constexpr int size = block_type::size;

    // Les blocs peuvent stocker un type T si il peut être contenu dans le bloc
    // et si il ne nécessite pas un plus grand alignement que 8.
    template <class T>
    static constexpr bool can_be_contained =
        sizeof (T) <= size &&
        alignof(T) <= 8;
    
    template <class T>
    block_allocator<T, block_resource> get_allocator() noexcept {
        return { *this };
    }

    // On alloue la liste chaînée de blocs, et on fait pointer chaque bloc
    // vers le bloc suivant.
    explicit block_resource(int nbBlocks) :
            blocks_(nbBlocks),
            head_(&blocks_.front())
    {
        for (int i = 0; i < nbBlocks - 1; ++i) {
            blocks_[i].next = &blocks_[i + 1];
        }
        blocks_.back().next = nullptr;
    }

    void* allocate() {
        if (head_ == nullptr) throw std::overflow_error
            {"The block allocator do not have any block yet"};
        
        auto const head = head_;
        auto const next = head_->next;
        head_ = next;
        return head;
    }

    void deallocate(void* ptr) noexcept {
        auto const block = static_cast<block_type*>(ptr);
        block->next = head_;
        head_ = block;
    }
private:
    std::vector<block_type, Allocator> blocks_;
    block_type* head_;
};


// Pour améliorer la ressource, on pourrait modifier l'alignement
// via un paramètre template.
// On pourrait également allouer un nouveau tableau de blocs lorsque
// plus aucun n'est disponible, au lieu de lancer une erreur.
