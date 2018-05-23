
#pragma once

#include <catch.hpp>
#include <stdexcept>
#include <string>
#include <cassert>
#include <iostream>

// Enregistre le nombre d'opérations effectuées.
struct op_data {
    int ctors;
    int moves;
    int copies;
    int dtors;

    bool operator==(op_data const& data) const noexcept {
        return ctors  == data.ctors  && 
               moves  == data.moves  && 
               copies == data.copies &&
               dtors  == data.dtors;
    }
};

std::ostream& operator<<(std::ostream& os, op_data const& data) {
    os << "{ ";
    os << "ctors = "  << data.ctors  << ", ";
    os << "moves = "  << data.moves  << ", ";
    os << "copies = " << data.copies << ", ";
    os << "dtors = "  << data.dtors;
    os << " }";
    return os;
}

class op_tracker {
    op_data& data;
public:
    inline op_tracker(op_data& data) noexcept : data(data) {
        ++data.ctors;
    }
    inline op_tracker(op_tracker&& rhs) noexcept : data(rhs.data) {
        ++data.moves;
    }
    inline op_tracker(op_tracker const& rhs) noexcept : data(rhs.data) {
        ++data.copies;
    }
    inline op_tracker& operator=(op_tracker&& rhs) noexcept {
        assert(&data == &rhs.data);
        ++data.moves;
        return *this;
    }
    inline op_tracker& operator=(op_tracker const& rhs) noexcept {
        assert(&data == &rhs.data);
        ++data.copies;
        return *this;
    }
    inline ~op_tracker() {
        ++data.dtors;
    }
};
