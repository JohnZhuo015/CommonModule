#ifndef OVERLOAD_LAMBDA_HPP
#define OVERLOAD_LAMBDA_HPP

#include <utility>
#include <type_traits>

template <typename... FunctionType>
struct OverloadLambda { };

template <typename T, typename... U>
struct OverloadLambda<T, U...>: T, OverloadLambda<U...> {
    explicit OverloadLambda(T _t, U... _u)
        : T(_t),
          OverloadLambda<U...>(_u...) { }

    using T::operator();
    using OverloadLambda<U...>::operator();
};

template <typename T>
struct OverloadLambda<T>: T {
    explicit OverloadLambda(T _t)
        : T(_t) { }

    using T::operator();
};

template <typename... LambdaType>
OverloadLambda<LambdaType...> MakeOverloadLambda(LambdaType &&... lambdas) {
    return OverloadLambda<LambdaType...>(std::forward<LambdaType>(lambdas)...);
}

#endif
