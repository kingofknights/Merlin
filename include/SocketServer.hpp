#ifndef MERLIN_INCLUDE_SOCKETSERVER_HPP_
#define MERLIN_INCLUDE_SOCKETSERVER_HPP_
#pragma once

#include <boost/asio.hpp>

class Connection;
using ConnectionPtrT = std::shared_ptr<Connection>;
class SocketServer final {
  public:
    explicit SocketServer(uint16_t port_);

    void startAccept();

    void handleAccept(const boost::system::error_code& error_);

    void runServer();

  private:
    boost::asio::io_context        _ioContext;
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::socket   _socket;
    boost::system::error_code      _errorCode;
    ConnectionPtrT                 _connection;
};
#endif// MERLIN_INCLUDE_SOCKETSERVER_HPP_
