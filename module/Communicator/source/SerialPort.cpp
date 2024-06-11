#include "SerialPort.hpp"

SyncSerialPort::SyncSerialPort(boost::asio::io_service &_ioService, std::string &&_portName, uint32_t _baudRate)
      : SyncSerialPort(_ioService, std::forward<std::string>(_portName), _baudRate, false, false, 1) { }

SyncSerialPort::SyncSerialPort(boost::asio::io_service &_ioService, std::string &&_portName, uint32_t _baudRate, bool _enableParity, bool _enableFlowControl, uint8_t _stopBits)
    : ioService {_ioService},
      portName {std::move(_portName)},
      baudRate {_baudRate},
      parity {_enableParity
            ? boost::asio::serial_port::parity::odd
            : boost::asio::serial_port::parity::none},
      flowControl {_enableFlowControl
            ? boost::asio::serial_port::flow_control::software
            : boost::asio::serial_port::flow_control::none},
      stopBits {_stopBits == 1
            ? boost::asio::serial_port::stop_bits::one
            : boost::asio::serial_port::stop_bits::two},
      serialPort {ioService},
      syncSerialPortLogger(spdlog::rotating_logger_mt("sync_serial_port_logger", "log/module/Sync/SerialPort.log", 1024 * 1024 * 5, 10)) { }

void SyncSerialPort::Connect() try {
      this->serialPort.open(this->portName);
      this->serialPort.set_option(this->baudRate);
      this->serialPort.set_option(this->parity);
      this->serialPort.set_option(this->flowControl);
      this->serialPort.set_option(this->stopBits);
} catch(boost::system::system_error &e) {
      this->syncSerialPortLogger->LOG_ERROR(e.what());
}

void SyncSerialPort::Disconnect() try {
      this->serialPort.close();
} catch(boost::system::system_error &e) {
      this->syncSerialPortLogger->LOG_ERROR(e.what());
}

void SyncSerialPort::WriteData(const boost::asio::const_buffer &writeData) try {
      this->serialPort.write_some(writeData);
} catch(boost::system::system_error &e) {
      this->syncSerialPortLogger->LOG_ERROR(e.what());
}

void SyncSerialPort::WriteData(boost::asio::const_buffer &&writeData) try {
      this->serialPort.write_some(std::forward<boost::asio::const_buffer>(writeData));
} catch(boost::system::system_error &e) {
      this->syncSerialPortLogger->LOG_ERROR(e.what());
}

void SyncSerialPort::ReadData(const boost::asio::mutable_buffer &readData) try {
      this->serialPort.read_some(readData);
} catch(boost::system::system_error &e) {
      this->syncSerialPortLogger->LOG_ERROR(e.what());
}

void SyncSerialPort::ReadData(boost::asio::mutable_buffer &&readData) try {
      this->serialPort.read_some(std::forward<boost::asio::mutable_buffer>(readData));
} catch(boost::system::system_error &e) {
      this->syncSerialPortLogger->LOG_ERROR(e.what());
}
