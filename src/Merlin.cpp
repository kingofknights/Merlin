#include "../include/Merlin.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../include/Global.hpp"
#include "../include/Logger.hpp"
#include "../include/SocketServer.hpp"
extern AdaptorContainerT AdaptorContainer;
Merlin::Merlin() : _socketServerPtr(std::make_shared<SocketServer>(9090)) {
    Global::AdaptorLoader(_threadGroup, "DemoAdaptor.dll", Exchange_NSE_FUTURE);
}

Merlin::~Merlin() {
    for (AdaptorConnectionT& adaptorConnection_ : AdaptorContainer) {
        if (adaptorConnection_.AdaptorPtr) {
            adaptorConnection_.AdaptorPtr.reset();
        }
        if (adaptorConnection_.SharedLibPtr) {
            adaptorConnection_.SharedLibPtr->unload();
            adaptorConnection_.SharedLibPtr.reset();
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
