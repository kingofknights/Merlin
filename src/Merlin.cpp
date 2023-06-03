#include "../include/Merlin.hpp"

#include "../include/SocketServer.hpp"

Merlin::Merlin() {
	_socketServerPtr = std::make_shared<SocketServer>(54321);
}

void Merlin::run() {
	_socketServerPtr->startAccept();
	_socketServerPtr->runServer();
}
