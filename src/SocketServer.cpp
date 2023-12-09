#include "../include/SocketServer.hpp"

#include <Lancelot.hpp>
#include <boost/bind/bind.hpp>

#include "../include/Connection.hpp"

SocketServer::SocketServer(uint16_t port_) : _acceptor(_ioContext), _endpoint(boost::asio::ip::tcp::v4(), port_), _socket(_ioContext) {
    _acceptor.open(_endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(_endpoint);
    _acceptor.listen();
}

void SocketServer::startAccept() { _acceptor.async_accept(_socket, boost::bind(&SocketServer::handleAccept, this, boost::asio::placeholders::error)); }

void SocketServer::handleAccept(const boost::system::error_code& error_) {
    if (!error_) {
        _connection = std::make_shared<Connection>(std::move(_socket));
    }
    startAccept();
}

void SocketServer::runServer() {
    _ioContext.run(_errorCode);
    LOG(WARNING, "{} {}", __FUNCTION__, _errorCode.message())
}
