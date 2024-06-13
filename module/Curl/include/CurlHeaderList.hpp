#ifndef CURL_HEADER_LIST_HPP
#define CURL_HEADER_LIST_HPP

#include <memory>
#include "curl/curl.h"
#include "Span.hpp"

class CurlHeaderList {
public:
    using this_type = CurlHeaderList;
    using this_pointer = CurlHeaderList *;
    using value_type = curl_slist;
    using pointer = curl_slist *;

    explicit CurlHeaderList()
        : headerLists{nullptr} {}

    ~CurlHeaderList() noexcept {
        curl_slist_free_all(this->headerLists);
    }

    pointer GetPointer() const {
        return this->headerLists;
    }

    this_type &operator<<(std::string &&message) {
        curl_slist_append(this->headerLists, message.data());
        return *this;
    }

    friend this_type operator<<(const this_type &obj, Span<char> &&message) {
        curl_slist_append(obj.headerLists, message.data);
        return obj;
    }
    friend this_type operator<<(this_type &&obj, Span<char> &&message) {
        curl_slist_append(obj.headerLists, message.data);
        return obj;
    }
private:
    curl_slist *headerLists {};
};

#endif
