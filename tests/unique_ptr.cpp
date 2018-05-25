
#include <catch.hpp>
#include <unique_ptr.hpp>
#include "operations_tracker.hpp"

namespace {
    struct object {
        int value;
    };
}

TEST_CASE("unique_ptr basics", "[unique_ptr]") {
    auto ptr = unique_ptr<object>{};
    REQUIRE(!ptr);

    ptr = unique_ptr<object>{ new object{ 42 } };
    REQUIRE(ptr);
    ptr->value += 2;
    REQUIRE((*ptr).value == 44);

    auto ptr2 = std::move(ptr);
    auto const ptr3{ std::move(ptr2) };
    REQUIRE(!ptr);
    REQUIRE(!ptr2);
    REQUIRE(ptr3);
    REQUIRE(ptr3->value == 44);
}

TEST_CASE("unique_ptr operations", "[unique_ptr]") {
    op_data data{};
    {
        auto raw = new op_tracker{ data };
        auto ptr = unique_ptr<op_tracker>{ raw };
        REQUIRE(data == op_data{ 1, 0, 0, 0 }); // 1 ctor
        {
            auto const ptr2 = std::move(ptr);
            REQUIRE(data == op_data{ 1, 0, 0, 0 });

            unique_ptr<op_tracker> empty; // Test null ptr dtor
        }
        REQUIRE(data == op_data{ 1, 0, 0, 1 }); // +1 dtor
    }
    REQUIRE(data == op_data{ 1, 0, 0, 1 });
}
