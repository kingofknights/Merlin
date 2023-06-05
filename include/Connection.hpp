#pragma once

#include <boost/asio.hpp>
#include <memory>

class Connection {
public:
	explicit Connection(boost::asio::ip::tcp::socket socket_);

	void read();
	void writeAsync(char* buffer_, size_t size_);
	void write(char* buffer_, size_t size_);

protected:
	void handle_read(const boost::system::error_code& error_, size_t size_);
	void handle_write(const boost::system::error_code& error_, size_t size_);

private:
	enum { BUFFER_SIZE = 530, UNCOMPRESSION_BUFFER_SIZE = 2048 };
	boost::asio::ip::tcp::socket _socket;
	boost::system::error_code	 _errorCode;
	char						 _buffer[BUFFER_SIZE];
	uint64_t					 _userId = 0;
};
