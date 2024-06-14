#ifndef CURL_CALL_BACK_FUNCTION_HPP
#define CURL_CALL_BACK_FUNCTION_HPP

#include <algorithm>
#include <type_traits>
#include <cstring>

template <
    typename T,
    typename = std::enable_if_t<
        std::is_class_v<T>,
        decltype(std::declval<T>().data())
    >
> struct CBFunctionByWriteImpl{
    using pointer = decltype(std::declval<T>().data());
    using value_type = std::remove_pointer_t<pointer>;

    size_t operator()(void *src, size_t count, void *dst) {
        const auto &convertDst = reinterpret_cast<T *>(dst);
        // convertDst->clear();
        std::copy((pointer)src, (pointer)src + count, std::back_inserter(*convertDst));
        return count;
    }
};

template <typename T, size_t N>
struct CBFunctionByWriteImpl<T[N], std::enable_if_t<std::is_array_v<T> > > {
    size_t operator()(void *src, size_t count, void *dst) {
        const auto &convertDst = reinterpret_cast<T *>(dst);
        // memset(convertDst, ' ', N);
        std::copy((T *)src, (T *)src + std::min(N, count), convertDst);
        return count;
    }
};

template <
    typename T,
    typename = std::enable_if_t<
        std::is_class_v<T>,
        std::enable_if_t<
            std::is_pointer_v<decltype(std::declval<T>().data())>,
            std::enable_if_t<
                std::is_same_v<size_t, decltype(std::declval<T>().size())>
            >
        >
    >
> struct CBFunctionByReadImpl {
    using pointer = decltype(std::declval<T>().data());
    using value_type = std::remove_pointer_t<pointer>;

    size_t operator()(void *src, void *dst) {
        const auto &convertSrc = reinterpret_cast<T *>(src);
        std::copy(convertSrc->begin(), convertSrc->end(), (pointer)dst);
        return convertSrc->size();
    }
};

template <typename T, size_t N>
struct CBFunctionByReadImpl<T[N], std::enable_if_t<std::is_array_v<T> > > {
    size_t operator()(void *src, void *dst) {
        const auto &convertSrc = reinterpret_cast<T *>(src);
        std::copy(convertSrc, convertSrc + N, (T *)dst);
        return N;
    }
};

template <typename T>
size_t CBFunctionByWrite(void *src, size_t size, size_t count, void *dst) {
    return CBFunctionByWriteImpl<T>{}(src, count, dst);
}

template <typename T, size_t N>
size_t CBFunctionByWrite(void *src, size_t size, size_t count, void *dst) {
    return CBFunctionByWriteImpl<T[N]>()(src, count, dst);
}

template <typename T>
size_t CBFunctionByRead(void *dst, size_t size, size_t count, void *src) {
    return CBFunctionByReadImpl<T>{}(src, dst);
}

template <typename T, size_t N>
size_t CBFunctionByRead(void *dst, size_t size, size_t count, void *src) {
    return CBFunctionByReadImpl<T[N]>{}(src, dst);
}

#endif
