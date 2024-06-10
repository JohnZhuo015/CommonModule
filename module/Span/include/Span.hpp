#ifndef SPAN_HPP
#define SPAN_HPP

#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename ArrayType, typename valueType = typename ArrayType::value_type>
struct ConceptForArrayHasDataAndSizeFunction {
    using type = std::enable_if_t<
        std::is_convertible_v<decltype(std::declval<ArrayType>().data()), valueType *>,
        std::enable_if_t<
            std::is_same_v<decltype(std::declval<ArrayType>().size()), size_t>,
            ArrayType
        >
    >;
};

template <typename ArrayType>
using has_data_and_size_function_t = typename ConceptForArrayHasDataAndSizeFunction<ArrayType>::type;

template <typename ValueType>
struct Span {
    using this_type = Span<ValueType>;
    using this_pointer = Span<ValueType> *;
    using value_type = ValueType;
    using pointer = ValueType *;
    using size_type = std::size_t;

    template <size_t N>
    explicit Span(value_type (&arr)[N])
        : data{arr},
          size{N} {}

    template <typename ArrayType>
    explicit Span(const has_data_and_size_function_t<ArrayType> &arr)
        : data{arr.data()},
          size{arr.size()} {}

    template <typename ArrayType>
    explicit Span(has_data_and_size_function_t<ArrayType> &&arr)
        : data{std::forward<ArrayType::pointer>(arr.data())},
          size{std::forward<ArrayType::size_type>(arr.size())} {}

    this_type Subspan(size_type start, size_type length = size_type(-1)) {
        if(start > this->size) {
            throw std::out_of_range("start point out of range, by subspan");
        }
        return {this->data + start, std::min(length, this->size - start)};
    }

    pointer data;
    size_type size;
};

#endif