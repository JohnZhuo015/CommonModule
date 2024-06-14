#include <iostream>
#include <array>
#include <vector>
#include <functional>
#include <cxxabi.h>
#include "boost/type_index.hpp"
#include "Span.hpp"
#include "Curl.hpp"
#include "CurlCallBackFunction.hpp"
#include "CurlOption.hpp"

#define TYPE_NAME_B(T) boost::typeindex::type_id_with_cvr<T>().pretty_name()
#define TYPE_NAME_S(type_id) abi::__cxa_demangle(type_id.name(), nullptr, nullptr, nullptr)

template <typename T>
size_t CBFunction(void *src, size_t size, size_t count, void *dst) {
    using pointer = decltype(std::declval<T>().data());
    using value_type = std::remove_pointer_t<pointer>;

    const auto &convertDst = reinterpret_cast<T *>(dst);
    convertDst->clear();
    std::copy((pointer)src, (pointer)src + count, std::back_inserter(*convertDst));

    return size * count;
}

// void TestCurlGet() {
//     CurlGet test;
//     test.SetURL("https://www.baidu.com");
//     std::string buf {}; buf.reserve(4096);
//     std::string hbuf {}; hbuf.reserve(4096);
//     test.GetPacketBody(buf, CBFunctionByWrite<std::string>);
//     test.GetPacketHeader(hbuf, CBFunctionByWrite<std::string>);
//     test.Send();
//
//     std::cout << "buf: " << buf << std::endl;
//     std::cout << "header buf: " << hbuf << std::endl;
// }

int main() {
    Curl<CurlMode::GET, CurlPacketRegion::BODY> test;
    test.SetUrl("https://www.baidu.com");
    std::string buf {}; buf.reserve(4096);
    test.GetPacket(buf, CBFunctionByWrite<std::string>);
    std::cout << CurlCodeMessage(test.Send()) << '\n';
    std::cout << "buf: " << buf << "\n";
}
