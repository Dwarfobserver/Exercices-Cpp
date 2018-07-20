
#pragma once

#include <todo.hpp>
#include <memory>
#include <tuple>
#include <vector>


template <class T, class Allocator = std::allocator<T>>
class slot_map {
public:
    class id_type {
        friend class slot_map;
        int value;
        id_type() = default;
        id_type(int value) noexcept : value(value) {}
    public:
        bool operator==(id_type rhs) const noexcept { return value == rhs.value; }
        bool operator!=(id_type rhs) const noexcept { return value != rhs.value; }
        bool operator< (id_type rhs) const noexcept { return value <  rhs.value; }
        bool operator<=(id_type rhs) const noexcept { return value <= rhs.value; }
        bool operator> (id_type rhs) const noexcept { return value >  rhs.value; }
        bool operator>=(id_type rhs) const noexcept { return value >= rhs.value; }
    };
private:
    struct data_t {
        id_type id;
        int pos;
    };
    
    template <class U>
    using rebound_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<U>;
    using data_allocator = rebound_allocator<data_t>;
    using id_allocator   = rebound_allocator<id_type>;

    std::vector<T,       Allocator>      objects;
    std::vector<data_t,  data_allocator> infos;
    std::vector<id_type, id_allocator>   freeIds;
public:
    using value_type     = T;
    using iterator       = typename std::vector<T, Allocator>::iterator;
    using const_iterator = typename std::vector<T, Allocator>::const_iterator;

    slot_map(Allocator const& allocator = Allocator()) :
        objects(allocator),
        infos  (data_allocator{ allocator }),
        freeIds(id_allocator  { allocator })
    {}

    void reserve(int capacity) {
        objects.reserve(capacity);
        infos  .reserve(capacity);
        freeIds.reserve(capacity);
    }

    int size() const noexcept { return objects.size(); }

    int capacity() const noexcept { return objects.capacity(); }
    
    template <class...Args>
    [[nodiscard]]
    std::pair<id_type, T&> emplace(Args&&...args) {
        auto const size = this->size();
        id_type id;
        if (freeIds.empty()) {
            id.value = size;
            infos.push_back({ id, size });
        }
        else {
            id = freeIds.back();
            freeIds.pop_back();
            infos[id.value].pos = size;
            infos[size    ].id  = id;
        }
        auto& val = objects.emplace_back(std::forward<Args>(args) ...);
        return { id, val };
    }

    void erase(id_type id) noexcept(std::is_nothrow_move_assignable_v<T>) {
        auto const erasedPos = infos[id.value].pos;
        auto erased = &objects[erasedPos];
        auto last   = &objects.back();
        if (erased != last) {
            *erased = std::move_if_noexcept(*last);
        }
        objects.pop_back();
        auto const size = this->size();
        infos[id.value] = { size, -1 };
        infos[size    ] = { -1  , id.value };
        freeIds.push_back(id);
    }

    iterator       begin()       noexcept { return objects.begin(); }
    const_iterator begin() const noexcept { return objects.begin(); }

    iterator       end()       noexcept { return objects.end(); }
    const_iterator end() const noexcept { return objects.end(); }

    T& operator[](id_type id) noexcept {
        auto const pos = infos[id.value].pos;
        return objects[pos];
    }
    T const& operator[](id_type id) const noexcept {
        auto const pos = infos[id.value].pos;
        return objects[pos];
    }
};
