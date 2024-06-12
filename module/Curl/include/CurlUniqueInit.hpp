#ifndef CURL_UNIQUE_INITIALIZATION_HPP
#define CURL_UNIQUE_INITIALIZATION_HPP

#include <string>
#include "CurlInitFlags.hpp"

class CurlUniqueInit {
public:
    using this_type = CurlUniqueInit;
    using this_pointer = CurlUniqueInit *;
    using pointer = std::unique_ptr<this_type>;
private:
    explicit CurlUniqueInit()
        : CurlUniqueInit(CurlInitFlags::ALL) {}
    explicit CurlUniqueInit(CurlInitFlags::type flags)
        : code{curl_global_init(CurlInitFlags::CurlInitFlagsMapping(flags))} {}
    ~CurlUniqueInit() noexcept {
        curl_global_cleanup();
    }
public:
    explicit CurlUniqueInit(const this_type &) = delete;
    explicit CurlUniqueInit(this_type &&) = delete;
    this_type &operator=(const this_type &) = delete;
    this_type &operator=(this_type &&) = delete;
public:
    static pointer Create(CurlInitFlags::type flags = CurlInitFlags::ALL) {
        return std::make_unique<this_type>(flags);
    }
public:
    CURLcode code;
};

inline std::string CurlCodeMessage(CURLcode code) {
    return {curl_easy_strerror(code)};
}

#endif
