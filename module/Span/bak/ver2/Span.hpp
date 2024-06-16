#ifndef SPAN_HPP
#define SPAN_HPP

#include <type_traits>
#include <cstdint>
#include <stdexcept>

template <typename T, typename = void>
struct has_data: public std::false_type {};

template <typename T>
struct has_data<T, std::void_t<decltype(std::declval<T>().data())> >: public std::true_type {};

template <typename T, typename = void>
struct Span {
    using pointer = decltype(std::declval<T>().data());
    using value_type = typename std::remove_pointer<pointer>::type;
    using size_type = std::size_t;

    explicit Span(const T &arr)
        : data_{const_cast<pointer>(arr.data())},
          size_{arr.size()} {}

    explicit Span(T &&arr)
        : data_{arr.data()},
          size_{arr.size()} {}

    pointer begin() {
        return this->data_;
    }

    pointer end() {
        return this->data_ + this->size_;
    }

    pointer data() {
        return this->data_;
    }

    size_t size() {
        return this->size_;
    }

    pointer data_;
    size_type size_;
};

template <typename T>
struct Span<T, typename std::enable_if<!has_data<T>::value && std::is_array<T>::value>::type> {
    using value_type = typename std::remove_pointer<typename std::remove_all_extents<T>::type>::type;
    using pointer = value_type *;
    using size_type = std::size_t;

    template <std::size_t N>
    explicit Span(value_type (&arr)[N])
        : data_{arr},
          size_{N} {}

    template <std::size_t N>
    explicit Span(const value_type (&arr)[N])
        : data_{const_cast<pointer>(arr)},
          size_{N} {}

    pointer begin() {
        return this->data_;
    }

    pointer end() {
        return this->data_ + this->size_;
    }

    pointer data() {
        return this->data_;
    }

    size_t size() {
        return this->size_;
    }

    pointer data_;
    size_type size_;
};

#endif
