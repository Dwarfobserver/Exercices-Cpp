
#pragma once

#include <tuple>


template <class T, class Tuple>
struct add_to {
    using type = void;
};

template <class T, class...Ts>
struct add_to<T, std::tuple<Ts...>> {
    using type = std::tuple<Ts..., T>;
};

template <class T, class Tuple>
struct remove_from {
    using type = void;
};

namespace detail {
    template <class T, class Tuple1, class Tuple2>
    struct remove_from {};

    template <class T, class U, class...T1, class...T2>
    struct remove_from<
        std::tuple<T1...>,
        T,
        std::tuple<U, T2...>
    > {
        using type = typename remove_from<
            std::tuple<T1..., U>,
            T,
            std::tuple<T2...>
        >::type;
    };

    template <class T, class...T1, class...T2>
    struct remove_from<
        std::tuple<T1...>,
        T,
        std::tuple<T, T2...>
    > {
        using type = std::tuple<T1..., T2...>;
    };

    template <class T, class...Ts>
    struct remove_from<
        std::tuple<Ts...>,
        T,
        std::tuple<>
    > {
        using type = std::tuple<Ts...>;
    };
}

template <class T, class...Ts>
struct remove_from<T, std::tuple<Ts...>> {
    using type = typename detail::remove_from<
        std::tuple<>,
        T,
        std::tuple<Ts...>
    >::type;
};

template <class T1, class T2>
struct is_smaller_than {
    static constexpr bool value = sizeof(T1) < sizeof(T2);
};

template <class Tuple, template <class, class> class Predicate>
struct sorted {
    using type = void;
};

namespace detail {

    template <class, class, class, template <class, class> class>
    struct insert {};
    template <class T, class U, class...T1, class...T2, template <class, class> class Predicate>
    struct insert<
        std::tuple<T1...>,
        T,
        std::tuple<U, T2...>,
        Predicate
    > {
        using type = std::conditional_t<
            Predicate<T, U>::value, 
            std::tuple<T1..., T, U, T2...>,
            typename insert<
                std::tuple<T1..., U>,
                T,
                std::tuple<T2...>,
                Predicate
            >::type
        >;
    };
    template <class T, class...Ts, template <class, class> class Predicate>
    struct insert<
        std::tuple<Ts...>,
        T,
        std::tuple<>,
        Predicate
    > {
        using type = std::tuple<Ts..., T>;
    };

    template <class, class, class, template <class, class> class>
    struct sorted {};
    template <class T, class...Sorted, class Next, class...Rest, template <class, class> class Predicate>
    struct sorted<
        std::tuple<Sorted...>,
        T,
        std::tuple<Next, Rest...>,
        Predicate
    > {
        using type = typename sorted<
            typename insert<
                std::tuple<>,
                T,
                std::tuple<Sorted...>,
                Predicate
            >::type,
            Next,
            std::tuple<Rest...>,
            Predicate
        >::type;
    };
    template <class T, class...Sorted, template <class, class> class Predicate>
    struct sorted<
        std::tuple<Sorted...>,
        T,
        std::tuple<>,
        Predicate
    > {
        using type = typename insert<
            std::tuple<>,
            T,
            std::tuple<Sorted...>,
            Predicate
        >::type;
    };
}

template <template <class, class> class Predicate>
struct sorted<std::tuple<>, Predicate> {
    using type = std::tuple<>;
};
template <class T, class...Ts, template <class, class> class Predicate>
struct sorted<std::tuple<T, Ts...>, Predicate> {
    using type = typename detail::sorted<
        std::tuple<>,
        T,
        std::tuple<Ts...>,
        Predicate
    >::type;
};
