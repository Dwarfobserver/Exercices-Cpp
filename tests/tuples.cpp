
#include <catch.hpp>
#include <tuples.hpp>

TEST_CASE("tuples add_to", "[tuples]") {
    using res1 =    add_to<int&, std::tuple<>>::type;
    REQUIRE(std::is_same_v<res1, std::tuple<int&>>);

    using res2 =    add_to<int,  std::tuple<char, float>>::type;
    REQUIRE(std::is_same_v<res2, std::tuple<char, float, int>>);
}

TEST_CASE("tuples remove_from", "[tuples]") {
    using res1 = remove_from<int, std::tuple<>>::type;
    REQUIRE(std::is_same_v <res1, std::tuple<>>);
    
    using res2 = remove_from<char, std::tuple<double, int>>::type;
    REQUIRE(std::is_same_v  <res2, std::tuple<double, int>>);
    
    using res3 = remove_from<char, std::tuple<char, char>>::type;
    REQUIRE(std::is_same_v  <res3, std::tuple<char>>);
    
    using res4 = remove_from<int, std::tuple<short, char, int, double, int, unsigned>>::type;
    REQUIRE(std::is_same_v <res4, std::tuple<short, char, double, int, unsigned>>);
}

TEST_CASE("tuples is_smaller_than", "[tuples]") {
    REQUIRE(is_smaller_than<int, double>::value == true);
    REQUIRE(is_smaller_than<int, short> ::value == false);
    REQUIRE(is_smaller_than<int, int>   ::value == false);
}

namespace {
    template <class T1, class T2>
    struct compare_tags {
        constexpr static bool value = T1::tag < T2::tag;
    };

    struct Mini   { enum { tag = -1 }; };
    struct Little { enum { tag = 0  }; };
    struct Medium { enum { tag = 10 }; };
    struct Huge   { enum { tag = 20 }; };
    struct Maxi   { enum { tag = 30 }; };
}

TEST_CASE("tuples sorted", "[tuples]") {
    using tuple1  = std::tuple<>;
    using sorted1 = std::tuple<>;
    using res1    = sorted<tuple1, compare_tags>::type;
    REQUIRE(std::is_same_v<res1, sorted1>);
    
    using tuple2  = std::tuple<Huge, Maxi, Little>;
    using sorted2 = std::tuple<Little, Huge, Maxi>;
    using res2    = sorted<tuple2, compare_tags>::type;
    REQUIRE(std::is_same_v<res2, sorted2>);

    using tuple3  = std::tuple<Medium, Little, Maxi, Mini, Huge>;
    using sorted3 = std::tuple<Mini, Little, Medium, Huge, Maxi>;
    using res3    = sorted<tuple3, compare_tags>::type;
    REQUIRE(std::is_same_v<res3, sorted3>);
}


