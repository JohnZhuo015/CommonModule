#ifndef CURL_CALL_BACK_FUNCTION_HPP
#define CURL_CALL_BACK_FUNCTION_HPP

#include "Span.hpp"
#include "FunctionTraits.hpp"

enum class ReadOrWrite: bool {
    Read,
    Write
};

template <ReadOrWrite Operator = ReadOrWrite::Read>
struct CurlCallBackFunc {
    using func_type = FunctionTraits<size_t(void *, size_t, size_t, void *)>;
    using type = std::function<func_type::function_type>;

    template <typename T>
    static type Generate(Span<T> span) {
        return [span](func_type::Arg_t<0> source, func_type::Arg_t<1> elemSize, func_type::Arg_t<2> elemCount, func_type::Arg_t<3> destination) mutable {
            auto upperBound = std::min(span.size, elemCount);
            std::copy(
                reinterpret_cast<typename Span<T>::pointer>(source),
                reinterpret_cast<typename Span<T>::pointer>(source) + upperBound,
                span.begin()
            );
            return elemSize * elemCount;
        };
    }
};

template <>
struct CurlCallBackFunc<ReadOrWrite::Write> {
    using func_type = FunctionTraits<size_t(void *, size_t, size_t, void *)>;
    using type = std::function<func_type::function_type>;

    template <typename T>
    static type Generate(Span<T> span) {
        return [span](func_type::Arg_t<0> destination, func_type::Arg_t<1> elemSize, func_type::Arg_t<2> elemCount, func_type::Arg_t<3> source) mutable {
            std::copy(
                span.begin(),
                span.end(),
                std::begin(reinterpret_cast<typename Span<T>::pointer>(destination))
            );
            return elemSize * elemCount;
        };
    }
};


#endif
