#pragma once

#include <boost/asio.hpp>
#include <memory>

class Connection;
using ConnectionPointerT = std::shared_ptr<Connection>;

class Connection {
public:
	Connection(boost::asio::ip::tcp::socket socket_);

	void read();
	void writeAynsc(char* buffer_, size_t size_);
	void write(char* buffer_, size_t size_);
	void handle_read(const char* buffer_, size_t size_);

protected:
	void handle_write(const boost::system::error_code& error_, size_t size_);

private:
	enum { BUFFER_SIZE = 512 };
	boost::asio::ip::tcp::socket _socket;
	boost::system::error_code	 _errorCode;
	char						 _buffer[BUFFER_SIZE];
};
