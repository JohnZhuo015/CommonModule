#include "Tcp.hpp"
#include <iostream>

SyncTcp::SyncTcp(boost::asio::io_service &_ioService, std::string &&_ip, uint16_t _port)
    : SyncTcp(_ioService, {boost::asio::ip::address::from_string(std::forward<std::string>(_ip)), _port}){ }

SyncTcp::SyncTcp(boost::asio::io_service &_ioService, boost::asio::ip::tcp::endpoint && _local)
    : ioService {_ioService},
      endPoint {_local},
      socket {ioService},
      syncTcpLogger {spdlog::rotating_logger_mt("sync_tcp_logger", "log/module/Sync/Tcp.log", 1024 * 1024 * 5, 10)}{}

void SyncTcp::WriteData(const boost::asio::const_buffer &writeData) try {
    this->socket.send(writeData);
} catch(boost::system::system_error &e) {
    this->syncTcpLogger->LOG_ERROR(e.what());
}

void SyncTcp::WriteData(boost::asio::const_buffer &&writeData) try {
    this->socket.send(std::forward<boost::asio::const_buffer>(writeData));
} catch(boost::system::system_error &e) {
    this->syncTcpLogger->LOG_ERROR(e.what());
}

void SyncTcp::ReadData(const boost::asio::mutable_buffer &readData) try {
    this->socket.receive(readData);
} catch(boost::system::system_error &e) {
    this->syncTcpLogger->LOG_ERROR(e.what());
}

void SyncTcp::ReadData(boost::asio::mutable_buffer &&readData) try {
    if(!this->socket.available()) { return ; }
    this->socket.receive(std::forward<boost::asio::mutable_buffer>(readData));
} catch(boost::system::system_error &e) {
    this->syncTcpLogger->LOG_ERROR(e.what());
}

boost::asio::ip::tcp::socket &SyncTcp::GetSocket() {
    return this->socket;
}
boost::asio::io_service &SyncTcp::GetIOService() {
    return this->ioService;
}
boost::asio::ip::tcp::endpoint &SyncTcp::GetEndPoint() {
    return this->endPoint;
}

std::shared_ptr<spdlog::logger> &SyncTcp::GetLogger() {
    return this->syncTcpLogger;
}

SyncTcpClient::SyncTcpClient(SyncTcp *_base)
    : base {_base} { }

SyncTcpClient::SyncTcpClient(std::unique_ptr<SyncTcp> &_base)
    : base {_base.release()} { }

SyncTcpClient::SyncTcpClient(std::unique_ptr<SyncTcp> &&_base)
    : base {std::move(_base)} { }

SyncTcpClient::SyncTcpClient(boost::asio::io_service &_ioService, std::string &&_ip, uint32_t _port)
    : SyncTcpClient(std::make_unique<SyncTcp>(_ioService, std::forward<std::string>(_ip), _port)) { }

void SyncTcpClient::Connect() try {
    this->base->GetSocket().connect(this->base->GetEndPoint());
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}

void SyncTcpClient::Disconnect() try {
    this->base->GetSocket().close();
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}

void SyncTcpClient::WriteData(const boost::asio::const_buffer &writeData) try {
    this->base->WriteData(writeData);
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}
void SyncTcpClient::WriteData(boost::asio::const_buffer &&writeData) try {
    this->base->WriteData(std::forward<boost::asio::const_buffer>(writeData));
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}
void SyncTcpClient::ReadData(const boost::asio::mutable_buffer &readData) try {
    this->base->ReadData(readData);
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}
void SyncTcpClient::ReadData(boost::asio::mutable_buffer &&readData) try {
    this->base->ReadData(std::forward<boost::asio::mutable_buffer>(readData));
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}

SyncTcpServer::SyncTcpServer(SyncTcp *_base)
    : base{_base},
      acceptor {base->GetIOService(), base->GetEndPoint()} { }

SyncTcpServer::SyncTcpServer(std::unique_ptr<SyncTcp> &_base)
    : base{_base.release()},
      acceptor {base->GetIOService(), base->GetEndPoint()} { }

SyncTcpServer::SyncTcpServer(std::unique_ptr<SyncTcp> &&_base)
    : base {std::move(_base)},
      acceptor {base->GetIOService(), base->GetEndPoint()} { }

SyncTcpServer::SyncTcpServer(boost::asio::io_service &_ioService, std::string &&_ip, uint32_t _port)
    : SyncTcpServer(std::make_unique<SyncTcp>(_ioService, std::forward<std::string>(_ip), _port)) { }

void SyncTcpServer::Connect() try {
    this->acceptor.accept(this->base->GetSocket());
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}

void SyncTcpServer::Disconnect() try {
    this->acceptor.close();
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}

void SyncTcpServer::WriteData(const boost::asio::const_buffer &writeData)  try{
    this->base->WriteData(writeData);
    this->base->GetSocket().close();
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}

void SyncTcpServer::WriteData(boost::asio::const_buffer &&writeData) try {
    this->base->WriteData(std::forward<boost::asio::const_buffer>(writeData));
    this->base->GetSocket().close();
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}

void SyncTcpServer::ReadData(const boost::asio::mutable_buffer &readData) try {
    this->base->ReadData(readData);
    this->base->GetSocket().close();
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}

void SyncTcpServer::ReadData(boost::asio::mutable_buffer &&readData) try {
    std::cout << "client" << this->base->GetSocket().remote_endpoint().address() << "\n";
    this->base->ReadData(std::forward<boost::asio::mutable_buffer>(readData));
    this->base->GetSocket().close();
} catch(boost::system::system_error &e) {
    this->base->GetLogger()->LOG_ERROR(e.what());
}
