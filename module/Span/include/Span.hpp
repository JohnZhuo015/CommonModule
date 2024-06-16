#ifndef SPAN_VERSION_1_hpp
#define SPAN_VERSION_1_hpp

#include <type_traits>
#include <stdexcept>

template <typename, typename = void>
struct concept_has_data_function: public std::false_type { };

template <typename T>
struct concept_has_data_function<
    T,
    typename std::enable_if<
        std::is_class<T>::value,
        typename std::enable_if<
            std::is_pointer<decltype(std::declval<T>().data())>::value
        >::type
    >::type
>: public std::true_type { };

template <typename, typename>
struct concept_has_size_function: public std::false_type { };

template <typename T>
struct concept_has_size_function<
    T,
    typename std::enable_if<
        std::is_class<T>::value,
        typename std::enable_if<
            std::is_same<std::size_t, decltype(std::declval<T>().size())>::value
        >::type
    >::type
>: public std::true_type { };

template <typename T>
struct traits_data_type {
    using pointer = typename std::conditional<
        std::is_pointer<T>::value,
        T,
        typename std::enable_if<
            concept_has_data_function<T>::value,
            decltype(std::declval<T>().data())
        >::type
    >::type;
    using value_type = typename std::remove_pointer<pointer>::type;
};

template <typename T, size_t N>
struct traits_data_type<T[N]> {
    using value_type = typename std::remove_reference<typename std::remove_cv<T>::type>::type;
    using pointer = typename std::add_pointer<value_type>::type;
    static constexpr std::size_t size = N;
};

template <typename T>
struct Span {
    using pointer = typename traits_data_type<T>::pointer;
    using value_type = typename std::remove_pointer<pointer>::type;
    using size_type = std::size_t;

    explicit Span(const T &arr)
        : data_{const_cast<pointer>(arr.data())},
          size_{arr.size()} { }

    explicit Span(T &&arr)
        : data_{arr.data()},
          size_{arr.size()} { }

    explicit Span(pointer _data, size_type _size)
        : data_{_data},
          size_{_size} { }

    pointer begin() noexcept {
        return this->data_;
    }

    pointer end() noexcept {
        return this->data_ + this->size_;
    }

    pointer data() const noexcept {
        return this->data_;
    }

    pointer data() noexcept {
        return this->data_;
    }

    constexpr size_type size() const {
        return this->size_;
    }

    auto Subspan(size_type start, size_type length = SIZE_MAX) {
        if(start >= this->size_) {
            throw std::out_of_range("start pointer out of range, by Subspan");
        }
        return Span{this->data() + start, std::min(this->size_ - start, length)};
    }

private:
    pointer data_{};
    size_type size_{};
};

template <typename T, size_t N>
struct Span<T[N]> {
    using value_type = typename traits_data_type<T[N]>::value_type;
    using pointer = typename traits_data_type<T[N]>::pointer;
    using size_type = std::size_t;

    explicit Span(T (&arr)[N])
        : data_{arr} { }

    explicit Span(const T (&arr)[N])
        : data_{arr} { }

    pointer begin() noexcept {
        return this->data_;
    }

    pointer end() noexcept {
        return this->data_ + this->size_;
    }

    pointer data() const noexcept {
        return this->data_;
    }

    pointer data() noexcept {
        return this->data_;
    }

    constexpr size_type size() const {
        return this->size_;
    }

private:
    pointer data_{};
    static constexpr size_type size_{N};
};

#endif
