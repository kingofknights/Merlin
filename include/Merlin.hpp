#pragma once
#include <boost/asio.hpp>

using ExecutorType = boost::asio::io_context::executor_type;
using WorkerT	   = boost::asio::executor_work_guard<ExecutorType>;

class Merlin {
public:
	Merlin();

	void run();

private:
	boost::asio::io_context _ioContext;
	WorkerT					_worker;
};
