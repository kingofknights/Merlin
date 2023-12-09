#ifndef MERLIN_INCLUDE_CONNECTION_HPP_
#define MERLIN_INCLUDE_CONNECTION_HPP_
#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <unordered_map>

namespace Lancelot {
struct CommunicationT;

namespace API {
    using StrategyParamT = std::unordered_map<std::string, std::string>;
}
}// namespace Lancelot

class ManualOrder;
using ManualOrderPtrT = std::shared_ptr<ManualOrder>;

class Connection : public std::enable_shared_from_this<Connection> {
  public:
    explicit Connection(boost::asio::ip::tcp::socket socket_);
    ~Connection();

    void read();
    void writeAsync(char* buffer_, size_t size_);
    void write(char* buffer_, size_t size_);

  protected:
    void handleRead(const boost::system::error_code& error_, size_t size_);
    void handleWrite(const boost::system::error_code& error_, size_t size_);

  private:
    void processQuery(const Lancelot::CommunicationT* communication_);

    void subscribe(uint32_t strategy_, const std::string& name_, const Lancelot::API::StrategyParamT& param_);
    void apply(uint32_t strategy_, const Lancelot::API::StrategyParamT& param_);

    void newOrder(const Lancelot::CommunicationT* communication_);
    void modifyOrder(const Lancelot::CommunicationT* communication_);
    void deleteOrder(const Lancelot::CommunicationT* communication_);

  private:
    enum : std::uint16_t {
        RAW_BUFFER_SIZE = 2048
    };
    unsigned char _rawBuffer[RAW_BUFFER_SIZE]{};
    int           _rawBufferSize = 0;
    size_t        _size          = 0;
    char*         _buffer        = nullptr;
    uint32_t      _userId        = 0;

    boost::asio::ip::tcp::socket _socket;
    boost::system::error_code    _errorCode;
    ManualOrderPtrT              _manualOrderPtr;
};

#endif// MERLIN_INCLUDE_CONNECTION_HPP_