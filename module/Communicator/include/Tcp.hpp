#ifndef TCP_HPP
#define TCP_HPP

#include "Communicator.hpp"

class SyncTcp: public SyncCommunicator {
public:
    SyncTcp(boost::asio::io_service &, std::string &&, uint16_t);
    SyncTcp(boost::asio::io_service &, boost::asio::ip::tcp::endpoint &&);
    ~SyncTcp() noexcept override = default;

    void Connect() override {}
    void Disconnect() override {}
    void WriteData(const boost::asio::const_buffer &) override;
    void WriteData(boost::asio::const_buffer &&) override;
    void ReadData(const boost::asio::mutable_buffer &) override;
    void ReadData(boost::asio::mutable_buffer &&) override;

    boost::asio::ip::tcp::socket &GetSocket();
    boost::asio::ip::tcp::endpoint &GetEndPoint();
    boost::asio::io_service &GetIOService();
    std::shared_ptr<spdlog::logger> &GetLogger();
private:
    boost::asio::io_service &ioService;
    boost::asio::ip::tcp::endpoint endPoint;
    boost::asio::ip::tcp::socket socket;
    std::shared_ptr<spdlog::logger> syncTcpLogger;
};

class SyncTcpClient: public SyncCommunicator {
public:
    SyncTcpClient(SyncTcp *);
    SyncTcpClient(std::unique_ptr<SyncTcp> &);
    SyncTcpClient(std::unique_ptr<SyncTcp> &&);
    SyncTcpClient(boost::asio::io_service &, std::string &&, uint32_t);
    ~SyncTcpClient() noexcept override = default;

    void Connect() override;
    void Disconnect() override;
    void WriteData(const boost::asio::const_buffer &) override;
    void WriteData(boost::asio::const_buffer &&) override;
    void ReadData(const boost::asio::mutable_buffer &) override;
    void ReadData(boost::asio::mutable_buffer &&) override;
private:
    std::unique_ptr<SyncTcp> base {};
};

class SyncTcpServer: public SyncCommunicator {
public:
    SyncTcpServer(SyncTcp *);
    SyncTcpServer(std::unique_ptr<SyncTcp> &);
    SyncTcpServer(std::unique_ptr<SyncTcp> &&);
    SyncTcpServer(boost::asio::io_service &, std::string &&, uint32_t);
    ~SyncTcpServer() noexcept override = default;

    void Connect() override;
    void Disconnect() override;
    void WriteData(const boost::asio::const_buffer &) override;
    void WriteData(boost::asio::const_buffer &&) override;
    void ReadData(const boost::asio::mutable_buffer &) override;
    void ReadData(boost::asio::mutable_buffer &&) override;
private:
    std::unique_ptr<SyncTcp> base {};
    boost::asio::ip::tcp::acceptor acceptor;
};

#endif