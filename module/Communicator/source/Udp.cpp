#include "Udp.hpp"

using namespace std::literals;

SyncUdp::SyncUdp(boost::asio::io_service &_ioService, boost::asio::ip::udp::endpoint &&_peer)
    : ioService {_ioService},
      peerEndPoint {std::move(_peer)},
      socket {ioService},
      syncUdpLogger {spdlog::rotating_logger_mt("sync_udp_log", "log/module/Sync/Udp.log"s, 1024 * 1024 * 5, 10)} { }

SyncUdp::SyncUdp(boost::asio::io_service &_ioService, std::string && ip, uint16_t port)
    : SyncUdp(_ioService, boost::asio::ip::udp::endpoint{boost::asio::ip::address::from_string(std::forward<std::string>(ip)), port}) { }

void SyncUdp::Connect() try {
    this->socket.open(boost::asio::ip::udp::v4());
} catch(boost::system::system_error &e) {
    this->syncUdpLogger->LOG_ERROR(e.what());
}

void SyncUdp::Disconnect() try {
    this->socket.close();
} catch(boost::system::system_error &e) {
    this->syncUdpLogger->LOG_ERROR(e.what());
}

void SyncUdp::WriteData(const boost::asio::const_buffer &writeData) try {
    this->socket.send_to(writeData, this->peerEndPoint);
} catch(boost::system::system_error &e) {
    this->syncUdpLogger->LOG_ERROR(e.what());
}

void SyncUdp::WriteData(boost::asio::const_buffer &&writeData) try {
    this->socket.send_to(std::forward<boost::asio::const_buffer>(writeData), this->peerEndPoint);
} catch(boost::system::system_error &e) {
    this->syncUdpLogger->LOG_ERROR(e.what());
}

void SyncUdp::ReadData(const boost::asio::mutable_buffer &readData) try {
    this->socket.receive_from(readData, this->peerEndPoint);
} catch(boost::system::system_error &e) {
    this->syncUdpLogger->LOG_ERROR(e.what());
}

void SyncUdp::ReadData(boost::asio::mutable_buffer &&readData) try {
    if(!this->socket.available()) { return; }
    this->socket.receive_from(std::forward<boost::asio::mutable_buffer>(readData), this->peerEndPoint);
} catch(boost::system::system_error &e) {
    this->syncUdpLogger->LOG_ERROR(e.what());
}