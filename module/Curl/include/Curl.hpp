#ifndef CURL_HPP
#define CURL_HPP

#include <iostream>
#include <functional>
#include <memory>
#include "CurlInitFlags.hpp"
#include "CurlHeaderList.hpp"
#include "CurlUniqueInit.hpp"
#include "CurlOption.hpp"
#include "FunctionTraits.hpp"

using curl_pointer = std::shared_ptr<CURL>;

#ifdef test_version
class CurlGet {
public:
    using callback_function_type = FunctionTraits<size_t(void *, size_t, size_t, void *)>;
    using function = callback_function_type::function_type;
    using function_pointer = callback_function_type::function_pointer;

    explicit CurlGet(CurlInitFlags::type _flags = CurlInitFlags::ALL)
        : curlUniqueInit{CurlUniqueInit::Create(_flags)},
          curlPointer{curl_easy_init(), curl_easy_cleanup} {
        curl_easy_setopt(this->curlPointer.get(), CURLOPT_HTTPGET, 1L);
    }

    CURLcode SetURL(std::string &&url) {
        return curl_easy_setopt(this->curlPointer.get(), CURLOPT_URL, url.c_str());
    }

    template <typename T>
    void GetPacketBody(const T &buf, const std::function<function> &cbFunc) {
        curl_easy_setopt(this->curlPointer.get(), CURLOPT_WRITEDATA, &buf);
        curl_easy_setopt(this->curlPointer.get(), CURLOPT_WRITEFUNCTION, *cbFunc.target<function_pointer>());
    }

    template <typename T>
    void GetPacketHeader(const T &buf, const std::function<function> &cbFunc) {
        curl_easy_setopt(this->curlPointer.get(), CURLOPT_HEADERDATA, &buf);
        curl_easy_setopt(this->curlPointer.get(), CURLOPT_HEADERFUNCTION, *cbFunc.target<function_pointer>());
    }
    
    CURLcode SetHeaderList(CurlHeaderList &&headerList) {
        return curl_easy_setopt(this->curlPointer.get(), CURLOPT_HEADER, headerList.GetPointer());
    }

    CURLcode Send() {
        return curl_easy_perform(this->curlPointer.get());
    }

private:
    CurlUniqueInit::pointer curlUniqueInit;
    curl_pointer curlPointer;
};
#endif

template <CurlMode Mode, CurlPacketRegion ROI>
struct Curl
    : public CurlSetMode<Mode>,
      public CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI> {
public:
    explicit Curl()
        : Curl(CurlInitFlags::ALL) {}

    explicit Curl(CurlInitFlags::type initFlags)
        : curlUniqueInit_{CurlUniqueInit::Create(initFlags)} ,
          curlPointer_ {curl_easy_init(), curl_easy_cleanup} {
        CurlSetMode<Mode>::SetMode(this->curlPointer_);
    }

    CURLcode SetUrl(std::string &&url) const {
        if(url.empty()) { return CURLcode::CURLE_BAD_FUNCTION_ARGUMENT; }

        return curl_easy_setopt(this->curlPointer_.get(), CURLOPT_URL, url.data());
    }

    CURLcode SetHeaderList(CurlHeaderList &&headerList) const {
        return curl_easy_setopt(this->curlPointer_.get(), CURLOPT_HTTPHEADER, headerList.GetPointer());
    }

    CURLcode Send() const {
        return curl_easy_perform(this->curlPointer_.get());
    }

    template <typename T>
    void GetPacket(const T &buf, const std::function<typename CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>::function> &cbFunc) {
        CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>::GetPacket(this->curlPointer_, buf, cbFunc);
    }

    template <typename T>
    void GetPacket(const T &buf) {
        CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>::GetPacket(this->curlPointer_, buf, CBFunctionByWrite<T>);
    }
private:
    CurlUniqueInit::pointer curlUniqueInit_ {};
    curl_pointer curlPointer_ {};
};


#endif
