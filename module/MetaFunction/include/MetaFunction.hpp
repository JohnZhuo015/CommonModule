#ifndef META_FUNCTION_HPP
#define META_FUNCTION_HPP

#include <type_traits>

template <bool Cond, typename T, typename F>
struct meta_if {};

template <typename T, typename F>
struct meta_if<true, T, F> {
    using type = T;
};

template <typename T, typename F>
struct meta_if<false, T, F> {
    using type = F;
};

template <typename Cond, typename T, typename F>
struct meta_if_c {};

template <typename T, typename F>
struct meta_if_c<std::true_type, T, F> {
    using type = T;
};

template <typename T, typename F>
struct meta_if_c<std::false_type, T, F> {
    using type = F;
};

template <bool Cond, typename T, typename F>
using meta_if_t = meta_if<Cond, T, F>;

template <typename Cond, typename T, typename F>
using meta_if_c_t = meta_if_c<Cond, T, F>;

#endif
