#pragma once
#include <boost/asio.hpp>

#include "Structure.hpp"

class Merlin {
public:
	Merlin();

	~Merlin();

	void run();

protected:
	void AdaptorLoader(Exchange exchange_, std::string_view dllName_);

	void import(std::string_view path_);

private:
	SocketServerPtrT		_socketServerPtr;
	ThreadGroupT			_threadGroup;
	boost::asio::io_context _ioContext;
};
