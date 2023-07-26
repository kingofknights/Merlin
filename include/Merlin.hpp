#ifndef MERLIN_INCLUDE_MERLIN_HPP_
#define MERLIN_INCLUDE_MERLIN_HPP_
#pragma once
#include <boost/asio.hpp>

#include "Structure.hpp"

class Merlin {
public:
	Merlin();

	~Merlin();

	void run();

protected:
	void import(std::string_view path_);

private:
	SocketServerPtrT		_socketServerPtr;
	ThreadGroupT			_threadGroup;
	boost::asio::io_context _ioContext;
};

#endif	// MERLIN_INCLUDE_MERLIN_HPP_