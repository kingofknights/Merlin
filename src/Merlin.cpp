#include "../include/Merlin.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../API/Adaptor.hpp"
#include "../include/Logger.hpp"
#include "../include/SocketServer.hpp"
#include "../include/Strategy.hpp"

extern AdaptorContainerT AdaptorContainer;
Merlin::Merlin() : _socketServerPtr(std::make_shared<SocketServer>(9090)) {}
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

void Merlin::AdaptorLoader(Exchange exchange_, std::string_view dllName_) {
    LOG(INFO, "AdaptorLoader Requested {} {}", exchange_, dllName_)
    auto dll = std::make_unique<boost::dll::shared_library>(dllName_.data(), boost::dll::load_mode::rtld_lazy);

    LOG(INFO, "AdaptorLoader Opened {} {}", exchange_, dllName_)
    auto getDriver = dll->get<AdaptorPtrT(boost::asio::io_context&, ThreadGroupT&)>(ENTRY_FUNCTION_NAME);

    LOG(INFO, "AdaptorLoader Calling {} {} {}", exchange_, dllName_, ENTRY_FUNCTION_NAME)
    auto adaptor = getDriver(_ioContext, _threadGroup);
    LOG(INFO, "AdaptorLoader Called {} {} {}", exchange_, dllName_, ENTRY_FUNCTION_NAME)

    AdaptorContainer[exchange_].AdaptorPtr	 = adaptor;
    AdaptorContainer[exchange_].SharedLibPtr = std::move(dll);
}

void Merlin::import(std::string_view path_) {
    std::fstream file(path_.data(), std::ios::in);
    if (not file.is_open()) {
        LOG(ERROR, "Cannot open file : {}", path_);
        LOG(ERROR, "Aborting...! {}", "!!!");
        abort();
    }
}
