
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/abstractions/la-concurrence#challenges

#include <atomic>
#include <vector>

// La classe doit permettre à un consommateur et un producteur de se passer des objets
// de manière lock-free.
// Un ring buffer utilise un espace de mémoire continu pour y stocker les objets.

template <class T>
class ring_buffer {
public:
    // Le tableau est alloué avec une capacité fixe.
    explicit ring_buffer(int capacity) :
            head_(0),
            tail_(0),
            array_(capacity + 1)
    {}

    int capacity() const noexcept { return array_.size(); }

    // On avance le curseur du producteur et stocke l'objet construit avec les
    // arguments passés.
    template <class...Args>
    void emplace(Args&&...args) {
        int const head = head_;
        int const next = increment(head);

        // On choisit ici de boucler lorsque le ring_buffer est plein.
        int tail = tail_;
        while (next == tail) {
            tail = tail_.load();
        }

        new (array_.data() + head) T(std::forward<Args>(args)...);
        head_ = next;
    }

    // On avance le curseur du consommateur et retourne l'objet récupéré.
    T pop() {
        int const tail = tail_;

        int head = head_;
        while (tail == head) {
            head = head_.load();
        }

        T* ptr  = reinterpret_cast<T*>(array_.data() + tail);
        T value = std::move(*ptr);
        
        tail_ = increment(tail);
        return value;
    }
private:
    int increment(int i) const noexcept {
        return (i + 1) % capacity();
    }

    std::atomic<int> head_;
    std::atomic<int> tail_;
    std::vector<std::aligned_storage_t<sizeof(T), alignof(T)>> array_;
};

// Plusieurs améliorations sont possibles, notamment :
//  - Forcer la capacité à êter une puissance de deux, pour pouvoir
//    incrémenter les indices plus rapidement (bitmask vs modulo).
//  - Avoir un paramètre template qui décide du comportement à avoir
//    lorsque le ring_buffer est rempli.
//  - Utiliser des opérations atomiques relaxées.
