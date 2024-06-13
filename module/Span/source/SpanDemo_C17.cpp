#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <map>
#include <type_traits>

template <typename ArrayType, typename valueType = typename ArrayType::value_type>
struct ConceptBySpan {
    using type = std::enable_if_t<
        std::is_convertible_v<decltype(std::declval<ArrayType>().data()), valueType *>,
        std::enable_if_t<
            std::is_same_v<decltype(std::declval<ArrayType>().size()), size_t>,
            ArrayType
        >
    >;
};

template <typename T>
struct Span{
    using value_type = typename T::value_type;
    using pointer = value_type *;
    using size_type = std::size_t;

    template <typename = typename ConceptBySpan<T>::type>
    explicit Span(const T &arr)
        : data{const_cast<typename T::pointer>(arr.data())},
          size{arr.size()} {}

    template <typename = typename ConceptBySpan<T>::type>
    explicit Span(T &&arr)
        : data{arr.data()},
          size{arr.size()} {}

    pointer data;
    size_type size;
};

template <typename T>
struct Span<T *> {
    using value_type = T;
    using pointer = value_type *;
    using size_type = std::size_t;

    template <size_t N>
    explicit Span(T (&arr)[N])
        : data{arr},
          size{N} {}

    template <size_t N>
    explicit Span(const T (&arr)[N])
        : data{arr},
          size{N} {}

    pointer data;
    size_type size;
};

template <typename T>
Span(T[]) -> Span<T *>;

template <typename T>
Span(T(&)[]) -> Span<T *>;

int test_main() {
    std::string str {};
    std::vector<char> vec {};
    std::array<char, 1024> arr{};

    Span testStr(str);
    Span testRStr(std::string{});

    Span testVec(vec);
    Span testRVec(std::vector<char>{});

    Span testArr(arr);
    Span testRArr(arr);

    char carr[1024];
    Span testCArr(carr);

    return 0;
}
