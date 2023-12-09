#include "../include/Connection.hpp"

#include <Gawain.hpp>
#include <Lancelot.hpp>
#include <nlohmann/json.hpp>

#include "../include/Global.hpp"
#include "../include/ManualOrder.hpp"
#include "../include/Structure.hpp"

Connection::Connection(boost::asio::ip::tcp::socket socket_) : _size(sizeof(Lancelot::CommunicationT)), _buffer(new char[_size]), _socket(std::move(socket_)) {
    _socket.set_option(boost::asio::ip::tcp::no_delay(true));
    _socket.set_option(boost::asio::ip::tcp::socket::reuse_address(true));
    //_socket.set_option(boost::asio::ip::tcp::socket::linger(true, 5));
    //_socket.set_option(boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT>(true));
    //_socket.set_option(boost::asio::detail::socket_option::boolean<SOL_SOCKET, TCP_CORK>(true));

    read();
}

Connection::~Connection() {
    delete[] _buffer;
    LOG(WARNING, "{} {}", __PRETTY_FUNCTION__, _userId)
}

void Connection::read() {
    boost::asio::async_read(_socket, boost::asio::buffer(_buffer, _size), boost::asio::transfer_exactly(_size), [this](const boost::system::error_code& errorCode_, size_t size_) { handleRead(errorCode_, size_); });
}

void Connection::writeAsync(char* buffer_, size_t size_) {
    boost::asio::async_write(_socket, boost::asio::buffer(buffer_, size_), boost::asio::transfer_exactly(size_), [this](const boost::system::error_code& errorCode_, size_t length_) { handleWrite(errorCode_, length_); });
}

void Connection::write(char* buffer_, size_t size_) { boost::asio::write(_socket, boost::asio::buffer(buffer_, size_), boost::asio::transfer_exactly(size_), _errorCode); }

void Connection::handleRead(const boost::system::error_code& error_, size_t size_) {
    if (error_) {
        LOG(WARNING, "{} {} {}", __FUNCTION__, error_.message(), size_)
        Global::ConnectionClosed(_userId);
        return;
    }
    const auto* request = reinterpret_cast<const Lancelot::CommunicationT*>(_buffer);
    LOG(INFO, "Communication request {}", Lancelot::toString(static_cast<Lancelot::RequestType>(request->_query)))
    switch (request->_query) {
        case Lancelot::RequestType_LOGIN: {
            LOG(INFO, "New Login Connection {} {}", request->_query, request->_user)
            if (_userId == 0) {
                _userId = request->_user;
                Global::NewConnectionRequested(request->_user, shared_from_this());
                Global::AddressDemangler demangler(_userId, 9999);
                _manualOrderPtr = std::make_shared<ManualOrder>(demangler.getAddress());
            }
            break;
        };

        case Lancelot::RequestType_NEW:
        case Lancelot::RequestType_MODIFY:
        case Lancelot::RequestType_CANCEL: {
            std::memset(_rawBuffer, '\0', static_cast<size_t>(_rawBufferSize));
            switch (request->_query) {
                case Lancelot::RequestType_NEW: {
                    newOrder(request);
                    break;
                }
                case Lancelot::RequestType_MODIFY: {
                    modifyOrder(request);
                    break;
                }
                case Lancelot::RequestType_CANCEL: {
                    deleteOrder(request);
                    break;
                }
            }
            break;
        }

        case Lancelot::RequestType_SUBSCRIBE:
        case Lancelot::RequestType_APPLY:
        case Lancelot::RequestType_UNSUBSCRIBE:
        case Lancelot::RequestType_SUBSCRIBE_APPLY: {
            processQuery(request);
            break;
        }
        default:
            break;
    }
    read();
}
void Connection::handleWrite(const boost::system::error_code& error_, size_t size_) {
    if (error_) {
        LOG(WARNING, "{} {} {}", __FUNCTION__, error_.message(), size_)
    }
}
void Connection::processQuery(const Lancelot::CommunicationT* communication_) {
    std::memset(_rawBuffer, '\0', static_cast<size_t>(_rawBufferSize));
    Lancelot::Decrypt(communication_->_encryptMessage.data(), communication_->_encryptLength, _rawBuffer, &_rawBufferSize);
    std::stringstream ss;
    ss << _rawBuffer;
    LOG(INFO, "Strategy request {} {}", Lancelot::toString(static_cast<Lancelot::RequestType>(communication_->_query)), ss.str())

    const nlohmann::json  json     = nlohmann::json::parse(ss);
    const nlohmann::json& params   = json.at(JSON_PARAMS);
    uint32_t              strategy = params.at(JSON_PF_NUMBER).get<uint32_t>();
    std::string           name     = FORMAT("lib{}.so", params.at(JSON_STRATEGY_NAME).get<std::string>());

    if (communication_->_query == Lancelot::RequestType_UNSUBSCRIBE) {
        Global::AddressDemangler demangler(_userId, strategy);
        Global::StrategyStopRequest(demangler.getAddress());
        return;
    }

    const nlohmann::json&         arguments = params.at(JSON_ARGUMENTS);
    Lancelot::API::StrategyParamT strategyParam;
    for (auto iterator = arguments.cbegin(); iterator != arguments.cend(); ++iterator) {
        strategyParam.emplace(iterator.key(), iterator.value().get<std::string>());
    }

    switch (communication_->_query) {
        case Lancelot::RequestType_APPLY: {
            apply(strategy, strategyParam);
            break;
        }
        case Lancelot::RequestType_SUBSCRIBE: {
            subscribe(strategy, name, strategyParam);
            break;
        }
        case Lancelot::RequestType_SUBSCRIBE_APPLY: {
            subscribe(strategy, name, strategyParam);
            apply(strategy, strategyParam);
            break;
        }
    }
}
void Connection::subscribe(uint32_t strategy_, const std::string& name_, const Lancelot::API::StrategyParamT& param_) {
    Global::AddressDemangler demangler(_userId, strategy_);
    if (Global::StrategyLoader(name_, demangler.getAddress(), param_)) {
        std::string              status   = Global::GetStrategyStatus(strategy_);
        Lancelot::CommunicationT response = Lancelot::Encrypt(status, demangler.getConnectionId(), Lancelot::ResponseType_SUBCRIBED);
        writeAsync(reinterpret_cast<char*>(&response), sizeof(Lancelot::CommunicationT));
        LOG(INFO, "response {}", status)
    }
}
void Connection::apply(uint32_t strategy_, const Lancelot::API::StrategyParamT& param_) {
    Global::AddressDemangler demangler(_userId, strategy_);
    if (Global::StrategyParamUpdate(demangler.getAddress(), param_)) {
        std::string              status   = Global::GetStrategyStatus(strategy_);
        Lancelot::CommunicationT response = Lancelot::Encrypt(status, demangler.getConnectionId(), Lancelot::ResponseType_APPLIED);
        writeAsync(reinterpret_cast<char*>(&response), sizeof(Lancelot::CommunicationT));
        LOG(INFO, "response {}", status)
    }
}

void Connection::newOrder(const Lancelot::CommunicationT* communication_) {
    Lancelot::Decrypt(communication_->_encryptMessage.data(), communication_->_encryptLength, _rawBuffer, &_rawBufferSize);
    std::stringstream ss;
    ss << _rawBuffer;
    nlohmann::json  newOrder = nlohmann::json::parse(ss);
    nlohmann::json& params   = newOrder.at(JSON_PARAMS);

    int                      id       = newOrder.at(JSON_ID).get<int>();
    uint32_t                 token    = params.at(JSON_TOKEN).get<uint32_t>();
    float                    price    = std::stof(params.at(JSON_PRICE).get<std::string>());
    int                      quantity = params.at(JSON_QUANTITY).get<int>();
    std::string              client   = params.at(JSON_CLIENT).get<std::string>();
    Lancelot::Side           side     = params.at(JSON_SIDE).get<Lancelot::Side>();
    Lancelot::API::OrderType type     = params.at(JSON_ORDER_TYPE).get<Lancelot::API::OrderType>();

    if (_manualOrderPtr) {
        int classId = _manualOrderPtr->newOrder(token, side, client, "", type == Lancelot::API::OrderType_IOC);
        // FIXME : set multiplier from resultSet
        _manualOrderPtr->placeOrder(classId, static_cast<int>(price * 100), quantity, Lancelot::API::OrderRequest_NEW);
    }

    if (false) {
        nlohmann::json response;
        nlohmann::json param;
        response[JSON_ID]         = ++id;
        param[JSON_PF_NUMBER]     = communication_->_user;
        param[JSON_TOKEN]         = token;
        param[JSON_UNIQUE_ID]     = id;
        param[JSON_QUANTITY]      = quantity;
        param[JSON_FILL_QUANTITY] = 0;
        param[JSON_FILL_PRICE]    = 0;
        param[JSON_REMAINING]     = quantity;
        param[JSON_ORDER_ID]      = rand();
        param[JSON_PRICE]         = price;
        param[JSON_SIDE]          = side;
        param[JSON_CLIENT]        = client;
        param[JSON_TIME]          = Global::TimeStampToHReadable(std::chrono::system_clock::now().time_since_epoch().count());
        param[JSON_MESSAGE]       = "New Order Success";
        response[JSON_PARAMS]     = param;

        auto response_ = Lancelot::Encrypt(response.dump(), _userId, Lancelot::ResponseType_NEW);
        writeAsync(reinterpret_cast<char*>(&response_), sizeof(Lancelot::CommunicationT));
        LOG(INFO, "{} Request {}", __FUNCTION__, ss.str())
        LOG(INFO, "{} Response {}", __FUNCTION__, response.dump())
    }
}

void Connection::modifyOrder(const Lancelot::CommunicationT* communication_) {
    Lancelot::Decrypt(communication_->_encryptMessage.data(), communication_->_encryptLength, _rawBuffer, &_rawBufferSize);
    std::stringstream ss;
    ss << _rawBuffer;

    const nlohmann::json  modifyOrder = nlohmann::json::parse(ss);
    const nlohmann::json& params      = modifyOrder.at(JSON_PARAMS);

    float price     = std::stof(params.at(JSON_PRICE).get<std::string>());
    int   quantity  = params.at(JSON_QUANTITY).get<int>();
    int   unique_id = params.at(JSON_UNIQUE_ID).get<int>();

    if (_manualOrderPtr) _manualOrderPtr->placeOrder(unique_id, static_cast<int>(price * 100.0F), quantity, Lancelot::API::OrderRequest_MODIFY);
}
void Connection::deleteOrder(const Lancelot::CommunicationT* communication_) {
    Lancelot::Decrypt(communication_->_encryptMessage.data(), communication_->_encryptLength, _rawBuffer, &_rawBufferSize);
    std::stringstream ss;
    ss << _rawBuffer;

    const nlohmann::json  cancelOrder = nlohmann::json::parse(ss);
    const nlohmann::json& params      = cancelOrder.at(JSON_PARAMS);
    int                   id          = cancelOrder.at(JSON_ID).get<int>();
    long                  order_id    = params.at(JSON_ORDER_ID).get<long>();
    int                   unique_id   = params.at(JSON_UNIQUE_ID).get<int>();
    LOG(INFO, "Cancel Order {} {} {}", id, order_id, unique_id)
    if (_manualOrderPtr) _manualOrderPtr->placeOrder(unique_id, 0, 0, Lancelot::API::OrderRequest_CANCEL);
}
