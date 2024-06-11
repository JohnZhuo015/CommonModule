#ifndef COMMUNICATOR_FACTORY_HPP
#define COMMUNICATOR_FACTORY_HPP

#include "Communicator.hpp"
#include "SerialPort.hpp"
#include "Tcp.hpp"
#include "Udp.hpp"

class SyncCommunicatorFactory {
public:
    SyncCommunicatorFactory() = default;
    virtual ~SyncCommunicatorFactory() noexcept = default;

    template <CommunicatorType Type>
    static std::unique_ptr<SyncCommunicator> Create(boost::asio::io_service & ioService, std::string &&ipOrPortName, uint32_t portOrBaud) {
        switch(Type) {
#define GENERATE(z, n, d) case CommunicatorType:: BOOST_PP_ARRAY_ELEM(n, COMMUNICATOR_TYPE): return std::make_unique<BOOST_PP_CAT(Sync, BOOST_PP_ARRAY_ELEM(n, COMMUNICATOR_TYPE))>(ioService, std::forward<std::string>(ipOrPortName), portOrBaud);
            BOOST_PP_REPEAT(BOOST_PP_ARRAY_SIZE(COMMUNICATOR_TYPE), GENERATE, COMMUNICATOR_TYPE)
#undef GENERATE
            default:
                return std::make_unique<SyncTcpServer>(ioService, std::forward<std::string>(ipOrPortName), portOrBaud);
        }
    }
};

#endif