#ifndef CURL_OPTION_HPP
#define CURL_OPTION_HPP

#include <memory>
#include "curl/curl.h"
#include "CurlCallBackFunc.hpp"

using curl_pointer = std::shared_ptr<CURL>;
inline curl_pointer CreateCurlPointer() {
    return std::make_shared<CURL>(curl_easy_init(), curl_easy_cleanup);
}

enum class CurlMode {
    GET,
    POST,
    PUT
};

enum class CurlResonse {
    HEADER,
    BODY,
    HEADER_AND_BODY
};

enum class CurlBinarySwitch {
    ENABLE,
    DISABLE
};

template <CurlMode Mode = CurlMode::GET>
struct CurlSetMode {
    static CURLcode SetMode(const curl_pointer &curlPointer) {
        return curl_easy_setopt(curlPointer.get(), CURLOPT_HTTPGET, 1L);
    }
};

template <>
struct CurlSetMode<CurlMode::POST> {
    static CURLcode SetMode(const curl_pointer &curlPointer) {
        return curl_easy_setopt(curlPointer.get(), CURLOPT_HTTPGET, 1L);
    }
    static void SetData(const curl_pointer &curlPointer, Span<char> buf) {
        curl_easy_setopt(curlPointer.get(), CURLOPT_POSTFIELDSIZE, buf.size);
        curl_easy_setopt(curlPointer.get(), CURLOPT_POSTFIELDS, buf.data);
    }
};

template<CurlResonse ROI = CurlResonse::HEADER>
struct CurlGetResponse {
    using func_type = CurlCallBackFunc<char, ReadOrWrite::Read>::type;

    void BindROI(const curl_pointer &curlPointer, Span<char> buf) {
        this->callBackFunc = CurlCallBackFunc<char, ReadOrWrite::Read>::Generate(buf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERFUNCTION, this->callBackFunc);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERDATA, buf.data);
    }

    func_type callBackFunc {};
};

template <>
struct CurlGetResponse<CurlResonse::BODY> {
    using func_type = CurlCallBackFunc<char, ReadOrWrite::Read>::type;

    void BindROI(const curl_pointer &curlPointer, Span<char> buf) {
        this->callBackFunc = CurlCallBackFunc<char, ReadOrWrite::Read>::Generate(buf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEFUNCTION, this->callBackFunc);
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEDATA, buf.data);
    }

    func_type callBackFunc {};
};

template <>
struct CurlGetResponse<CurlResonse::HEADER_AND_BODY> {
    using func_type = CurlCallBackFunc<char, ReadOrWrite::Read>::type;

    void BindROI(const curl_pointer &curlPointer, Span<char> headerBuf, Span<char> bodyBuf) {
        this->bodyCallBackFunc = CurlCallBackFunc<char, ReadOrWrite::Read>::Generate(bodyBuf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEFUNCTION, this->bodyCallBackFunc);

        this->headerCallBackFunc = CurlCallBackFunc<char, ReadOrWrite::Read>::Generate(headerBuf);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERFUNCTION, this->headerCallBackFunc);

        curl_easy_setopt(curlPointer.get(), CURLOPT_WRITEDATA, bodyBuf.data);
        curl_easy_setopt(curlPointer.get(), CURLOPT_HEADERDATA, headerBuf.data);
    }

    func_type bodyCallBackFunc {};
    func_type headerCallBackFunc {};
};

template <CurlBinarySwitch = CurlBinarySwitch::ENABLE>
struct CurlSetTimeOut {
    static CURLcode SetTimeLimit(const curl_pointer &curlPointer, uint32_t limit) {
        return curl_easy_setopt(curlPointer.get(), CURLOPT_TIMEOUT, limit);
    }
};

template <>
struct CurlSetTimeOut<CurlBinarySwitch::DISABLE> {};

#endif
