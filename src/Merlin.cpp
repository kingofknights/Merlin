#include "../include/Merlin.hpp"

#include <Lancelot.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../include/Global.hpp"
#include "../include/SocketServer.hpp"
#include "../include/Structure.hpp"

namespace MerlinShared {
extern AdaptorContainerT                              _globalAdaptorContainer;
extern boost::asio::io_context                        _globalStrategyThread;
static std::unique_ptr<boost::asio::io_context::work> _globalWorker;
}// namespace MerlinShared

Merlin::Merlin() {
    LOG(INFO, "{}", __FUNCTION__)
    load("merlin.json");
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
void Merlin::load(std::string_view path_) {
    std::fstream file(path_.data(), std::ios::in);
    if (not file.is_open()) {
        LOG(ERROR, "Cannot open file : {}", path_);
        LOG(ERROR, "Aborting...! {}", "!!!");
        std::exit(EXIT_SUCCESS);
    }

    nlohmann::json json = nlohmann::json::parse(file);
    if (json.contains("server")) {
        int port = json.at("server").get<int>();
        LOG(INFO, "Starting Merlin socket server on port {}", port)
        _socketServerPtr = std::make_shared<SocketServer>(port);
    }
    if (json.contains("database")) {
        Lancelot::ResultSetLoadingCallbackT callback = [](Lancelot::ResultSetPtrT, float, float, float) {};
        Lancelot::ContractInfo::Initialize(json.at("database").get<std::string>(), callback);
    }

    if (json.contains("adaptors")) {
        auto adaptorList = json.at("adaptors");

        for (const auto& adaptor : adaptorList) {
            std::cout << "adaptor " << adaptor << std::endl;
            const std::string name = adaptor.at("exchange").get<std::string>();
            const std::string path = adaptor.at("path").get<std::string>();

            Global::AdaptorLoader(_threadGroup, name, Lancelot::Exchange_NSE_FUTURE);
        }
    }
}
