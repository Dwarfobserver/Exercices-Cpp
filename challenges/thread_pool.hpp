
// Solution du challenge présenté ici :
// https://cours-cpp.gitbook.io/resources/abstractions/la-concurrence#challenges

#include <future>
#include <thread>
#include <vector>
#include <functional>


class thread_pool {

    // On va lancer des threads qui vont se bloquer sur la même 'condition variable'
    // tant qu'aucun travail n'est disponible.
    // Lorsqu'ils sont notifiés, ils exécutent tout le travail disponible, puis
    // s'arrêtent si la thread_pool est en train d'être détruite.
    void work() {
        std::function<void()> f;
        while (true) {
            {
                std::unique_lock l{ mutex_ };
                cv_.wait(l, [this] { return !tasks_.empty() || terminated_; });
                if (tasks_.empty() && terminated_) return;
                f = tasks_.back();
                tasks_.pop_back();
            }
            f();
        }
    }
public:
    explicit thread_pool(int nbWorkers) :
            terminated_(false)
    {
        for (int i = 0; i < nbWorkers; ++i)
            workers_.emplace_back([this] { work(); });
    }

    // La variable doit être modifiée lorsque le mutex est lock, même si
    // elle est atomique, afin d'être synchronisée avec les workers qui
    // la vérifient.
    // Puis, tous les threads sont notifiés et attendus.
    ~thread_pool() {
        {
            std::lock_guard l{ mutex_ };
            terminated_ = true;
        }
        cv_.notify_all();
        for (auto& w : workers_) w.join();
    }

    // Notifie un thread après avoir actualisé la liste des tâches.
    template <class F>
    void enqueue(F&& f) {
        {
            std::lock_guard l{ mutex_ };
            tasks_.push_back(std::forward<F>(f));
        }
        cv_.notify_one();
    }
private:
    bool terminated_;
    std::vector<std::thread> workers_;
    std::vector<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
};
