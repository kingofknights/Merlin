#ifndef MERLIN_INCLUDE_MERLIN_HPP_
#define MERLIN_INCLUDE_MERLIN_HPP_
#pragma once

#include <memory>
#include <thread>
#include <vector>

class SocketServer;
using SocketServerPtrT = std::shared_ptr<SocketServer>;
using ThreadPointerT   = std::unique_ptr<std::jthread>;
using ThreadGroupT	   = std::vector<ThreadPointerT>;

class Merlin final {
public:
	Merlin();

	~Merlin();

	void run();

protected:
	void load(std::string_view path_);

private:
	int				 _maxStrategyThread = 5;
	SocketServerPtrT _socketServerPtr;
	ThreadGroupT	 _threadGroup;
};

#endif	// MERLIN_INCLUDE_MERLIN_HPP_