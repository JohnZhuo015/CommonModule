#ifndef CURL_CALL_BACK_FUNCTION_HPP
#define CURL_CALL_BACK_FUNCTION_HPP

#include "Span.hpp"
#include "FunctionTraits.hpp"

enum class ReadOrWrite: bool {
    Read,
    Write
};

template <typename ValueType, ReadOrWrite Operator>
struct CurlCallBackFunc;

template <typename ValueType>
struct CurlCallBackFunc<ValueType, ReadOrWrite::Read> {
    using func_type = FunctionTraits<void(void *, size_t, size_t, void *)>;
    using type = std::function<func_type::function_type>;

    static type Generate(Span<ValueType> span) {
        return [span](func_type::Arg_t<0> source, func_type::Arg_t<1> elemSize, func_type::Arg_t<2> elemCount, func_type::Arg_t<3> destination) {
            auto upperBound = std::min(span.size, elemCount);
            std::copy(
                reinterpret_cast<typename Span<ValueType>::pointer>(source),
                reinterpret_cast<typename Span<ValueType>::pointer>(source) + upperBound,
                std::back_inserter(span.data)
            );
        };
    }
};

template <typename ValueType>
struct CurlCallBackFunc<ValueType, ReadOrWrite::Write> {
    using func_type = FunctionTraits<void(void *, size_t, size_t, void *)>;
    using type = std::function<func_type::function_type>;

    static type Generate(Span<ValueType> span) {
        return [span](func_type::Arg_t<0> destination, func_type::Arg_t<1> elemSize, func_type::Arg_t<2> elemCount, func_type::Arg_t<3> source) {
            std::copy(
                span.data,
                span.data + span.size,
                std::back_inserter(reinterpret_cast<typename Span<ValueType>::value_type>(destination))
            );
        };
    }
};


#endif
