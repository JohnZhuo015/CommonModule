#ifndef UDP_HPP
#define UDP_HPP

#include "Communicator.hpp"

class SyncUdp: public SyncCommunicator {
public:
    SyncUdp(boost::asio::io_service &, boost::asio::ip::udp::endpoint &&);
    SyncUdp(boost::asio::io_service &, std::string &&, uint16_t);
    ~SyncUdp() noexcept override = default;

    void Connect() override;
    void Disconnect() override;
    void WriteData(const boost::asio::const_buffer &) override;
    void WriteData(boost::asio::const_buffer &&) override;
    void ReadData(const boost::asio::mutable_buffer &) override;
    void ReadData(boost::asio::mutable_buffer &&) override;
private:
    boost::asio::io_service &ioService;
    boost::asio::ip::udp::endpoint peerEndPoint;
    boost::asio::ip::udp::socket socket;
    std::shared_ptr<spdlog::logger> syncUdpLogger;
};

#endif