#ifndef FUNCTION_TRAITS_HPP
#define FUNCTION_TRAITS_HPP

#include <type_traits>
#include <tuple>
#include <functional>

template <typename T>
struct FunctionTraits;

template <typename ResultType, typename... Args>
struct FunctionTraits<ResultType(Args...)> {
    static constexpr const std::size_t ArgumentSize = sizeof...(Args);
    using result_type = ResultType;
    using function_type = ResultType(Args...);
    using stl_function_type = std::function<function_type>;
    using function_pointer = ResultType(*)(Args...);

    template <std::size_t ArgsIndex>
    struct Arg {
        static_assert(ArgsIndex < ArgumentSize);
        using type = typename std::tuple_element<ArgsIndex, std::tuple<Args...> >::type;
    };

    using tuple_type = std::tuple<std::decay_t<Args>...>;
};

template <typename ResultType, typename... Args>
struct FunctionTraits<ResultType(*)(Args...)>: FunctionTraits<ResultType(Args...)> { };

template <typename ResultType, typename... Args>
struct FunctionTraits<std::function<ResultType(Args...)> >: FunctionTraits<ResultType(Args...)> { };

template <typename ClassType, typename ResultType, typename... Args>
struct FunctionTraits<ResultType(ClassType::*)(Args...)>: FunctionTraits<ResultType(Args...)> { };

template <typename ClassType, typename ResultType, typename... Args>
struct FunctionTraits<ResultType(ClassType::*)(Args...) const>: FunctionTraits<ResultType(Args...)> { };

template <typename ClassType, typename ResultType, typename... Args>
struct FunctionTraits<ResultType(ClassType::*)(Args...) volatile>: FunctionTraits<ResultType(Args...)> { };

template <typename ClassType, typename ResultType, typename... Args>
struct FunctionTraits<ResultType(ClassType::*)(Args...) const volatile>: FunctionTraits<ResultType(Args...)> { };

template <typename Callable>
struct FunctionTraits: FunctionTraits<typename std::decay<decltype(&Callable::operator())>::type> { };

template <typename FunctionType>
typename FunctionTraits<FunctionType>::stl_function_type ToFunction(const FunctionType &lambda) {
    return static_cast<typename FunctionTraits<FunctionType>::stl_function_type>(lambda);
}

template <typename FunctionType>
typename FunctionTraits<FunctionType>::stl_function_type ToFunction(FunctionType &&lambda) {
    return static_cast<typename FunctionTraits<FunctionType>::stl_function_type>(std::forward<FunctionType>(lambda));
}

template <typename FunctionType>
typename FunctionTraits<FunctionType>::function_pointer ToFunctionPointer(const FunctionType &lambda) {
    return static_cast<typename FunctionTraits<FunctionType>::function_pointer>(lambda);
}

template <typename FunctionType>
typename FunctionTraits<FunctionType>::function_pointer ToFunctionPointer(FunctionType &&lambda) {
    return static_cast<typename FunctionTraits<FunctionType>::function_pointer>(std::forward<FunctionType>(lambda));
}

#endif
