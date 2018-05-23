
#include <catch.hpp>
#include <slot_map.hpp>
#include "operations_tracker.hpp"
#include <vector>
#include <map>

TEST_CASE("slot_map basics", "[slot_map]") {
    using id_t = slot_map<int>::id_type;

    slot_map<int> ints;
    std::map<id_t, int> intsCopy;

    REQUIRE(ints.size() == 0);
    REQUIRE(ints.capacity() == 0);
    REQUIRE(ints.begin() == ints.end());

    auto add = [&] (int x) {
        auto [id, ref] = ints.emplace(x);
        // Empêche les erreurs de type 'ref non utilisé'.
        static_cast<void>(ref);
        intsCopy[id] = x;
    };
    auto remove = [&] (id_t id) {
        ints.erase(id);
        intsCopy.erase(id);
    };
    auto mapsEquality = [&] () -> bool {
        if (ints.size() != intsCopy.size()) return false;
        for (auto pair : intsCopy) {
            auto [id, x] = pair;
            if (ints[id] != x) return false;
        }
        return true;
    };
    
    add(4);
    add(7);
    add(10);
    REQUIRE(ints.size() == 3);
    REQUIRE(ints.capacity() >= 3);
    REQUIRE(mapsEquality());

    auto key = (*++intsCopy.begin()).first;
    remove(key);
    REQUIRE(ints.size() == 2);
    REQUIRE(mapsEquality());

    add(8);
    add(2);
    remove(intsCopy.begin()->first);
    remove(intsCopy.begin()->first);
    add(1);
    add(2);
    add(3);
    REQUIRE(mapsEquality());
}

TEST_CASE("slot_map operations", "[slot_map]") {
    using id_t = slot_map<op_tracker>::id_type;

    op_data data{}; 
    slot_map<op_tracker> map;
    std::vector<id_t> ids;

    map.reserve(2);
    ids.push_back(map.emplace(data).first);
    ids.push_back(map.emplace(data).first);
    REQUIRE(data == op_data{ 2, 0, 0, 0 }); // 2 ctors
    
    ids.push_back(map.emplace(data).first);
    REQUIRE(data == op_data{ 3, 2, 0, 2 }); // + 2 moves, 2 dtors
    
    map.erase(ids[1]);
    REQUIRE(data == op_data{ 3, 3, 0, 3 }); // + 1 move, 1 dtor
    {
        slot_map<op_tracker> map3;
        {
            auto map2 = map;
            REQUIRE(data == op_data{ 3, 3, 2, 3 }); // + 2 copies

            map3 = std::move(map2);
            REQUIRE(data == op_data{ 3, 3, 2, 3 }); // no changes
        }
    }
    REQUIRE(data == op_data{ 3, 3, 2, 5 }); // + 2 dtors
}
