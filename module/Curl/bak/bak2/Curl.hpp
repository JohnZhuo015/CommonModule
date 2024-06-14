#ifndef CURL_HPP
#define CURL_HPP

#include <memory>
#include "CurlHeaderList.hpp"
#include "CurlInitFlags.hpp"
#include "CurlUniqueInit.hpp"
#include "CurlOption.hpp"
#include "MetaFunction.hpp"

template <CurlMode Mode, CurlPacketRegion ROI, CurlBinarySwitch Switch>
class Curl
    : public CurlSetMode<Mode>,
      public CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>,
      public CurlSetTimeOut<Switch> {
public:
    using this_type = Curl;
    using this_pointer = Curl *;

    using function = meta_if_t<
            ROI == CurlPacketRegion::BODY || ROI == CurlPacketRegion::HEADER,
            typename CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>::function,
            meta_if_t<
                ROI == CurlPacketRegion::HEADER_AND_BODY,
                std::pair<
                    typename CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>::function,
                    typename CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>::function
                >,
                void
            >
        >;

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
    void SetPacket(Span<T> buf) {
        CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>::SetPacket(this->curlPointer_, buf, this->callBackFunc_);
    }

    template <typename T>
    void GetPacket(Span<T> buf) {
        CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>::GetPacket(this->curlPointer_,buf, this->callBackFunc_);
    }

    template <typename T, typename U = T>
    void GetPacket(Span<T> bodyBuf, Span<U> headerBuf) {
        CurlPacket<CurlSetMode<Mode>::PACKET_OPERATE, ROI>::GetPacket(this->curlPointer_, bodyBuf, headerBuf, this->callBackFunc_);
    }
private:
    CurlUniqueInit::pointer curlUniqueInit_ {};
    curl_pointer curlPointer_ {};
    function callBackFunc_ {};
};

#endif
