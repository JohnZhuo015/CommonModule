#include <array>
#include <iostream>
#include <typeindex>
#include <type_traits>
#include <vector>
#include <map>
#include <cxxabi.h>
#include "boost/type_index.hpp"

#define TYPE_NAME_B(T) boost::typeindex::type_id_with_cvr<T>().pretty_name()
#define TYPE_NAME_S(type_id) abi::__cxa_demangle(type_id, nullptr, nullptr, nullptr)

template <typename, typename = void>
struct concept_has_data_function { };

template <typename T>
struct concept_has_data_function<
    T,
    typename std::enable_if<
        std::is_class<T>::value,
        typename std::enable_if<
            std::is_pointer<decltype(std::declval<T>().data())>::value
        >::type
    >::type
> {
    using type = typename std::remove_cvref<T>::type;
    using pointer = typename std::decay<decltype(std::declval<type>().data())>::type;
    using value_type = typename std::remove_pointer<pointer>::type;
};

template <typename, typename>
struct concept_has_size_function { };

template <typename T>
struct concept_has_size_function<
    T,
    typename std::enable_if<
        std::is_class<T>::value,
        typename std::enable_if<
            std::is_same<std::size_t, decltype(std::declval<T>().size())>::value
        >::type
    >::type
> {
    using type = typename std::remove_cvref<T>::type;
    using size_type = std::size_t;
};

int main() {
    char arr[1024];
    std::cout << TYPE_NAME_B(decltype(arr)) << '\n';
    std::cout << TYPE_NAME_B(concept_has_data_function<std::vector<int>>::type) << '\n';
}
