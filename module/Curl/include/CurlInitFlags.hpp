#ifndef CURL_INITIALIZATION_FLAGS_HPP
#define CURL_INITIALIZATION_FLAGS_HPP

#include "curl/curl.h"

#define MY_CONCAT_INNER(x, y) x##y
#define MY_CONCAT(x, y) MY_CONCAT_INNER(x, y)

#undef WIN32
#define CURL_INIT_FLAGS(f) \
    f(ALL) \
    f(SSL) \
    f(WIN32) \
    f(NOTHING) \

struct CurlInitFlags {
    using type = enum Flags {
#define GENERATE(v) v,
        CURL_INIT_FLAGS(GENERATE)
#undef GENERATE
    };

    static auto CurlInitFlagsMapping(CurlInitFlags::type flags) ->decltype(CURL_GLOBAL_ALL) {
        switch(flags) {
#define GENERATE(v) case CurlInitFlags::v: { return {MY_CONCAT(CURL_GLOBAL_, v)}; }
            CURL_INIT_FLAGS(GENERATE)
#undef GENERATE
            default: {
                return {CURL_GLOBAL_ALL};
            }
        }
    }
};

#endif
