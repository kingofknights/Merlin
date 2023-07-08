#include "../include/Merlin.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../include/Global.hpp"
#include "../include/Logger.hpp"
#include "../include/SocketServer.hpp"

namespace details {
	extern AdaptorContainerT _globalAdaptorContainer;
}
Merlin::Merlin() : _socketServerPtr(std::make_shared<SocketServer>(9090)) { Global::AdaptorLoader(_threadGroup, "DemoAdaptor.dll", Lancelot::Exchange_NSE_FUTURE); }

Merlin::~Merlin() {
	for (AdaptorConnectionT& adaptorConnection : details::_globalAdaptorContainer) {
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
		abort();
	}
}
