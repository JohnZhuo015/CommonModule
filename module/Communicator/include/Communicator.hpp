#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include "boost/asio.hpp"
#include "boost/asio/serial_port.hpp"
#include "boost/asio/ip/udp.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/ip/address.hpp"
#include "boost/preprocessor.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "fmt/format.h"

#if defined(__GNUC__)
    #define EXTEND_FORMAT() fmt::format("[\n\tfile name: {},\n\tfunction name: {}\n\tline: {}\n]->  ", __FILE__, __PRETTY_FUNCTION__, __LINE__)
#elif defined(_MSC_VER)
    #define EXTEND_FORMAT() fmt::format("[\n\tfile name: {},\n\tfunction name: {}\n\tline: {}\n]->  ", __FILE__, __FUNCSIG__, __LINE__)
#else
    #define EXTEND_FORMAT() fmt::format("[\n\tfile name: {},\n\tfunction name: {}\n\tline: {}\n]->  ", __FILE__, __func__, __LINE__)
#endif

#define LOG_INFO(...) info(EXTEND_FORMAT() + __VA_ARGS__)
#define LOG_WARN(...) warn(EXTEND_FORMAT() + __VA_ARGS__)
#define LOG_ERROR(...) error(EXTEND_FORMAT() + __VA_ARGS__)


#define DEFAULT_COMMUNITOR_COUNT 4
#define COMMUNICATOR_TYPE ( \
    DEFAULT_COMMUNITOR_COUNT, ( \
        SerialPort, \
        Udp, \
        TcpClient, \
        TcpServer \
    ) \
)

enum class CommunicatorType {
    BOOST_PP_ARRAY_ENUM(COMMUNICATOR_TYPE)
};

class SyncCommunicator {
public:
    SyncCommunicator() = default;
    virtual ~SyncCommunicator() noexcept = default;

    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual void WriteData(const boost::asio::const_buffer &) = 0;
    virtual void WriteData(boost::asio::const_buffer &&) = 0;
    virtual void ReadData(const boost::asio::mutable_buffer &) = 0;
    virtual void ReadData(boost::asio::mutable_buffer &&) = 0;
};

#endif