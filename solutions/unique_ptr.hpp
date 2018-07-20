
#pragma once

#include <todo.hpp>


template <class T>
class unique_ptr {
    T* ptr;
public:
    unique_ptr() noexcept : ptr(nullptr) { }

    explicit unique_ptr(T* ptr) noexcept : ptr(ptr) { }

    unique_ptr(unique_ptr&& rhs) noexcept : ptr(rhs.ptr) { rhs.ptr = nullptr; }

    unique_ptr& operator=(unique_ptr&& rhs) noexcept {
        delete ptr;
        ptr = rhs.ptr;
        rhs.ptr = nullptr;
        return *this;
    }

    ~unique_ptr() { delete ptr; }

    T*       operator->()       noexcept { return ptr; }
    T const* operator->() const noexcept { return ptr; }
    
    T&       operator*()       noexcept { return *ptr; }
    T const& operator*() const noexcept { return *ptr; }

    explicit operator bool() const noexcept { return ptr != nullptr; }
};

