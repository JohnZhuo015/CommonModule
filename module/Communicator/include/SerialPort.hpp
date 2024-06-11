#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "Communicator.hpp"

class SyncSerialPort: public SyncCommunicator {
public:
    SyncSerialPort(boost::asio::io_service &, std::string &&, uint32_t);
    SyncSerialPort(boost::asio::io_service &, std::string &&, uint32_t, bool, bool, uint8_t);
    ~SyncSerialPort() noexcept override = default;

    void Connect() override;
    void Disconnect() override;
    void WriteData(const boost::asio::const_buffer &) override;
    void WriteData(boost::asio::const_buffer &&) override;
    void ReadData(const boost::asio::mutable_buffer &) override;
    void ReadData(boost::asio::mutable_buffer &&) override;
private:
    boost::asio::io_service &ioService;
    std::string portName;
    boost::asio::serial_port::baud_rate baudRate;
    boost::asio::serial_port::parity parity;
    boost::asio::serial_port::flow_control flowControl;
    boost::asio::serial_port::stop_bits stopBits;
    boost::asio::serial_port serialPort;
    std::shared_ptr<spdlog::logger> syncSerialPortLogger;
};

#endif
