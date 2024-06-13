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
        : data{const_cast<pointer>(arr.data())},
          size{arr.size()} {}

    explicit Span(T &&arr)
        : data{arr.data()},
          size{arr.size()} {}

    Span Subspan(size_type start, size_type length = size_type(-1)) {
        if(start > this->size) {
            throw std::out_of_range("start point out of range, by subspan");
        }
        return {this->data + start, std::min(length, this->size - start)};
    }

    pointer data;
    size_type size;
};


template <typename T>
struct Span<T, typename std::enable_if<!has_data<T>::value && std::is_array<T>::value>::type> {
    using value_type = typename std::remove_pointer<typename std::remove_all_extents<T>::type>::type;
    using pointer = value_type *;
    using size_type = std::size_t;

    template <std::size_t N>
    explicit Span(value_type (&arr)[N])
        : data{arr},
          size{N} {}

    template <std::size_t N>
    explicit Span(const value_type (&arr)[N])
        : data{const_cast<pointer>(arr)},
          size{N} {}

    Span Subspan(size_type start, size_type length = size_type(-1)) {
        if(start > this->size) {
            throw std::out_of_range("start point out of range, by subspan");
        }
        return {this->data + start, std::min(length, this->size - start)};
    }

    pointer data;
    size_type size;
};

#endif
