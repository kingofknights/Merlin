#pragma once
#include <boost/asio.hpp>

class SocketServer;

using SocketServerPtrT = std::shared_ptr<SocketServer>;
class Merlin {
public:
	Merlin();

	void run();

private:
	SocketServerPtrT _socketServerPtr;
};
