#pragma once

#include <memory>
#include "curl/curl.h"
#include "Span.hpp"
#include "CurlCallBackFunction.hpp"
#include "CurlHeaderList.hpp"
#include "CurlUniqueInit.hpp"
#include "FunctionTraits.hpp"

using curl_pointer = std::shared_ptr<CURL>;

enum class CurlMode {
    GET,
    POST,
    PUT
};

enum class CurlPacketOperate {
    Set,
    Get
};

enum class CurlPacketRegion {
    HEADER,
    BODY,
    HEADER_AND_BODY
};

enum class CurlBinarySwitch {
    ENABLE,
    DISABLE
};

template <CurlMode = CurlMode::GET>
struct CurlSetMode {
    static constexpr CurlPacketOperate PACKET_OPERATE = CurlPacketOperate::Get;

    static CURLcode SetMode(const curl_pointer &curlPointer) {
        return curl_easy_setopt(curlPointer.get(), CURLOPT_HTTPGET, 1L);
    }
};

template <>
struct CurlSetMode<CurlMode::POST> {
    static constexpr CurlPacketOperate PACKET_OPERATE = CurlPacketOperate::Get;

    static CURLcode SetMode(const curl_pointer &curlPointer) {
        return curl_easy_setopt(curlPointer.get(), CURLOPT_MIMEPOST, 1L);
    }

    template <typename T>
    static void SetData(const curl_pointer &curlPointer, Span<T> buf) {
        curl_easy_setopt(curlPointer.get(), CURLOPT_POSTFIELDSIZE, buf.size);
        curl_easy_setopt(curlPointer.get(), CURLOPT_POSTFIELDS, buf.data);
    }
};

template <>
struct CurlSetMode<CurlMode::PUT> {
    static constexpr CurlPacketOperate PACKET_OPERATE = CurlPacketOperate::Set;

    static CURLcode SetMode(const curl_pointer &curlPointer) {
        return curl_easy_setopt(curlPointer.get(), CURLOPT_UPLOAD, 1L);
    }
};

template <CurlPacketOperate Operate, CurlPacketRegion Region>
struct CurlPacket {};

template <>
struct CurlPacket<CurlPacketOperate::Get, CurlPacketRegion::HEADER> {
    using callback_function_type = FunctionTraits<size_t(void *, size_t, size_t, void *)>;
    using function = callback_function_type::function_type;
    using function_pointer = callback_function_type::function_pointer;

    template <typename T>
    static void GetPacket(const curl_pointer &curlPointer, const T &buf, const std::function<function> &cbFunc) {
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERDATA, &buf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERFUNCTION, *cbFunc.target<function_pointer>());
    }
};

template <>
struct CurlPacket<CurlPacketOperate::Get, CurlPacketRegion::BODY> {
    using callback_function_type = FunctionTraits<size_t(void *, size_t, size_t, void *)>;
    using function = callback_function_type::function_type;
    using function_pointer = callback_function_type::function_pointer;

    template <typename T>
    static void GetPacket(const curl_pointer &curlPointer, const T &buf, const std::function<function> &cbFunc) {
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEDATA, &buf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEFUNCTION, *cbFunc.target<function_pointer>());
    }
};

template <>
struct CurlPacket<CurlPacketOperate::Get, CurlPacketRegion::HEADER_AND_BODY> {
    using callback_function_type = FunctionTraits<size_t(void *, size_t, size_t, void *)>;
    using function = callback_function_type::function_type;
    using function_pointer = callback_function_type::function_pointer;

    template <typename T, typename U>
    static void GetPacket(const curl_pointer &curlPointer, const T &bodyBuf, const U &headerBuf, const std::function<function> &bodyCBFunc, const std::function<function> &headerCBFunc) {
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEDATA, &bodyBuf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEFUNCTION, *bodyCBFunc.target<function_pointer>());
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERDATA, &headerBuf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERFUNCTION, *headerCBFunc.target<function_pointer>());
    }
};
