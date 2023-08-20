#include "../include/Merlin.hpp"

#include <Lancelot.hpp>
#include <fstream>

#include "../include/Global.hpp"
#include "../include/SocketServer.hpp"
#include "../include/Structure.hpp"

namespace MerlinShared {
	extern AdaptorContainerT							  _globalAdaptorContainer;
	extern boost::asio::io_context						  _globalStrategyThread;
	static std::unique_ptr<boost::asio::io_context::work> _globalWorker;
}  // namespace MerlinShared

Merlin::Merlin() : _socketServerPtr(std::make_shared<SocketServer>(9090)) {
	LOG(INFO, "{}", __FUNCTION__)
	Lancelot::ResultSetLoadingCallbackT callback = [](Lancelot::ResultSetPtrT, float, float, float) {};
	Lancelot::ContractInfo::Initialize("ResultSet.db3", callback);
	Global::AdaptorLoader(_threadGroup, "libDemoAdaptor.so", Lancelot::Exchange_NSE_FUTURE);

	{
		auto thread = std::make_unique<std::jthread>(Global::ArthurMessageManagerThread);
		_threadGroup.push_back(std::move(thread));
	}

	MerlinShared::_globalWorker = std::make_unique<boost::asio::io_context::work>(MerlinShared::_globalStrategyThread);
	for (int i = 0; i < _maxStrategyThread; ++i) {
		auto thread = std::make_unique<std::jthread>([id = i](std::stop_token stopToken_) {
			while (not stopToken_.stop_requested()) {
				boost::system::error_code errorCode;
				MerlinShared::_globalStrategyThread.run(errorCode);
				LOG(ERROR, "Strategy thread id # {} stopped with error : {}", id, errorCode.message());
			}
		});
		_threadGroup.push_back(std::move(thread));
	}
}

Merlin::~Merlin() {
	MerlinShared::_globalWorker.reset();
	for (auto& adaptorConnection : MerlinShared::_globalAdaptorContainer) {
		if (adaptorConnection._adaptorPtr) {
			adaptorConnection._adaptorPtr.reset();
		}
		if (adaptorConnection._sharedLibPtr) {
			adaptorConnection._sharedLibPtr->unload();
			adaptorConnection._sharedLibPtr.reset();
		}
	}
}

void Merlin::run() {
	_socketServerPtr->startAccept();
	_socketServerPtr->runServer();
}

void Merlin::import(std::string_view path_) {
	std::fstream file(path_.data(), std::ios::in);
	if (not file.is_open()) {
		LOG(ERROR, "Cannot open file : {}", path_);
		LOG(ERROR, "Aborting...! {}", "!!!");
		std::exit(EXIT_SUCCESS);
	}
}
