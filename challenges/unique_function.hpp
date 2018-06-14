
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/abstractions/les-move-semantics#challenges

#include <utility>

// Afin de pouvoir stocker n'importe quel type, on aura un pointeur vers l'objet
// alloué dynamiquement.
// On stockera un pointeur vers la fonction appelant sa méthode 'operator()(Args...)'.
// On aura un autre pointeur vers son destructeur.
// Puisque l'objet ne peut pas être copié, le move consistera simplement à passer
// le pointeur vers l'objet.


template <class> class unique_function;

// On spécialise la classe template pour avoir la notation semblable à une fonction.
template <class Res, class...Args>
class unique_function<Res(Args...)> {
    using destroy_f_t = void (*) (void* f);
    using invoke_f_t =  Res  (*) (void* f, Args&&...args);

    // Les deux fonctions stockées.
    template <typename F>
    static Res invoke_f(F* f, Args&&... args) {
        return (*f)(std::forward<Args>(args)...);
    }
    template <typename F>
    static void destroy_f(F* f) {
        f->~F();
        delete f;
    }

    template <typename F>
    static F* create(F&& f) {
        return new F(std::forward<F>(f));
    }
public:
    ~unique_function() noexcept {
        reset();
    }

    // Constructeurs.

    unique_function() noexcept :
            invoke_(nullptr),
            destroy_(nullptr),
            data_(nullptr)
    {}

    // Les fonctions sont réinterprétées pour correspondre au type
    // commun défini au-dessus, quelque soit le type F.
    template <class F>
    unique_function(F&& f) :
            invoke_ (reinterpret_cast<invoke_f_t>(invoke_f<F>)),
            destroy_(reinterpret_cast<destroy_f_t>(destroy_f<F>)),
            data_(create(std::forward<F>(f)))
    {}

    unique_function(unique_function&& f) noexcept :
            invoke_(f.invoke_),
            destroy_(f.destroy_),
            data_(f.data_)
    {
        f.data_ = nullptr;
    }

    // Assignement.

    unique_function& operator=(unique_function&& f) noexcept {
        reset();
        invoke_  = f.invoke_;
        destroy_ = f.destroy_;
        data_    = f.data_;
        f.data_  = nullptr;
    }

    // Utilitaires.

    Res operator()(Args&&...args) const {
        return invoke_(data_, std::forward<Args>(args)...);
    }

    bool is_empty() const noexcept {
        return data_ != nullptr;
    }

    void reset() noexcept {
        if (data_) {
            destroy_(data_);
            data_ = nullptr;
        }
    }
private:
    invoke_f_t invoke_;
    destroy_f_t destroy_;
    void* data_;
};
