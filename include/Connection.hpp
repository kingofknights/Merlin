#pragma once

#include <boost/asio.hpp>
#include <memory>

class Connection {
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
	enum {
		UNCOMPRESSION_BUFFER_SIZE = 2048
	};
	size_t						 _size;
	char*						 _buffer;
	uint64_t					 _userId = 0;
	boost::asio::ip::tcp::socket _socket;
	boost::system::error_code	 _errorCode;
};
