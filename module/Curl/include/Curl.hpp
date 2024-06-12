#ifndef CURL_HPP
#define CURL_HPP

#include <memory>
#include "CurlHeaderList.hpp"
#include "CurlInitFlags.hpp"
#include "CurlUniqueInit.hpp"
#include "CurlOption.hpp"

template <CurlMode Mode, CurlResonse ROI, CurlBinarySwitch Switch>
class Curl
    : public CurlSetMode<Mode>,
      public CurlGetResponse<ROI>,
      public CurlSetTimeOut<Switch> {
public:
    using this_type = Curl;
    using this_pointer = Curl *;

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
private:
    CurlUniqueInit::pointer curlUniqueInit_{CurlUniqueInit::Create()};
    curl_pointer curlPointer_ {CreateCurlPointer()};
};

#endif
