#include "../include/Connection.hpp"

#include <boost/bind/bind.hpp>

#include "../include/Logger.hpp"

Connection::Connection(boost::asio::ip::tcp::socket socket_) : _socket(std::move(socket_)) {
	read();
}

void Connection::read() {
	boost::asio::async_read(_socket, boost::asio::buffer(_buffer, BUFFER_SIZE), boost::asio::transfer_exactly(BUFFER_SIZE), [this](const boost::system::error_code &error_code_, size_t size_) {
		if (not error_code_) {
			handle_read(_buffer, size_);
			read();
		} else {
			LOG(WARNING, "{} {} {}", __FUNCTION__, error_code_.message(), size_)
		}
	});
}

void Connection::writeAynsc(char *buffer_, size_t size_) {
	boost::asio::async_write(_socket, boost::asio::buffer(buffer_, size_), boost::asio::transfer_exactly(size_),
							 boost::bind(&Connection::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::write(char *buffer_, size_t size_) {
	boost::asio::write(_socket, boost::asio::buffer(buffer_, size_), boost::asio::transfer_exactly(size_), _errorCode);
}

void Connection::handle_read(const char *buffer_, size_t size_) {
	LOG(INFO, "{} {} ", __FUNCTION__, size_)
}

void Connection::handle_write(const boost::system::error_code &error_, size_t size_) {
	if (error_) {
		LOG(WARNING, "{} {} {}", __FUNCTION__, error_.message(), size_)
	}
}
