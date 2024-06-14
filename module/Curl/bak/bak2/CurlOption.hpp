#ifndef CURL_OPTION_HPP
#define CURL_OPTION_HPP

#include <memory>
#include "curl/curl.h"
#include "CurlCallBackFunc.hpp"

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

using CurlTimeOutEnable = CurlBinarySwitch;

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

template <CurlPacketRegion Region>
struct CurlPacket<CurlPacketOperate::Set, Region> {
    using func_type = CurlCallBackFunc<ReadOrWrite::Write>;
    using function = typename func_type::type;

    template <typename T>
    static void SetPacket(const curl_pointer &curlPointer, Span<T> buf, function &callBackFunc) {
        callBackFunc = func_type::Generate(buf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_READFUNCTION, callBackFunc);
        curl_easy_setopt(curlPointer.get(), CURLOPT_READDATA, buf.data);
        curl_easy_setopt(curlPointer.get(), CURLOPT_INFILESIZE_LARGE, buf.size);
    }
};

template <>
struct CurlPacket<CurlPacketOperate::Get, CurlPacketRegion::HEADER> {
    using func_type = CurlCallBackFunc<ReadOrWrite::Read>;
    using function = typename func_type::type;

    template <typename T>
    static void GetPacket(const curl_pointer &curlPointer, Span<T> buf, function &callBackFunc) {
        callBackFunc = func_type::Generate(buf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERFUNCTION, callBackFunc);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERDATA, buf.data);
    }
};

template <>
struct CurlPacket<CurlPacketOperate::Get, CurlPacketRegion::BODY> {
    using func_type = CurlCallBackFunc<ReadOrWrite::Read>;
    using function = typename func_type::type;

    template <typename T>
    static void GetPacket(const curl_pointer &curlPointer, Span<T> buf, function &callBackFunc) {
        callBackFunc = func_type::Generate(buf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEFUNCTION, callBackFunc);
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEDATA, buf.data);
    }
};

template <>
struct CurlPacket<CurlPacketOperate::Get, CurlPacketRegion::HEADER_AND_BODY> {
    using func_type = CurlCallBackFunc<ReadOrWrite::Read>;
    using function = typename func_type::type;

    template <typename T, typename U = T>
    static void GetPacket(const curl_pointer &curlPointer, Span<T> bodyBuf, Span<U> headerBuf, std::pair<function, function> &callBackFunction) {
        callBackFunction.first = func_type::Generate(bodyBuf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEFUNCTION, callBackFunction.first);

        callBackFunction.second = func_type::Generate(headerBuf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERFUNCTION, callBackFunction.second);

        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEDATA, bodyBuf.data);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERDATA, headerBuf.data);
    }
};

template <CurlTimeOutEnable = CurlTimeOutEnable::ENABLE>
struct CurlSetTimeOut {
    static CURLcode SetTimeLimit(const curl_pointer &curlPointer, uint32_t limit) {
        return curl_easy_setopt(curlPointer.get(), CURLOPT_TIMEOUT, limit);
    }
};

template <>
struct CurlSetTimeOut<CurlTimeOutEnable::DISABLE> {};

#endif
