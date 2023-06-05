#include "../include/Connection.hpp"

#include <boost/bind/bind.hpp>
#include <nlohmann/json.hpp>

#include "../include/Compression.h"
#include "../include/Enums.hpp"
#include "../include/Global.hpp"
#include "../include/Logger.hpp"
#include "../include/Structure.hpp"

Connection::Connection(boost::asio::ip::tcp::socket socket_) : _socket(std::move(socket_)) {
	read();
}

void Connection::read() {
	boost::asio::async_read(_socket, boost::asio::buffer(_buffer, BUFFER_SIZE), boost::asio::transfer_exactly(BUFFER_SIZE),
							boost::bind(&Connection::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::writeAsync(char *buffer_, size_t size_) {
	boost::asio::async_write(_socket, boost::asio::buffer(buffer_, size_), boost::asio::transfer_exactly(size_),
							 boost::bind(&Connection::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::write(char *buffer_, size_t size_) {
	boost::asio::write(_socket, boost::asio::buffer(buffer_, size_), boost::asio::transfer_exactly(size_), _errorCode);
}

void Connection::handle_read(const boost::system::error_code &error_, size_t size_) {
	if (error_) {
		LOG(WARNING, "{} {} {}", __FUNCTION__, error_.message(), size_)
		Global::ConnectionClosed(_userId);
		return;
	}
	const auto *request = reinterpret_cast<const RequestInPackT *>(_buffer);
	switch (request->Type) {
		case RequestType_LOGIN: {
			LOG(INFO, "New Login Connection {} {}", request->Type, request->UserIdentifier)
			if (_userId == 0) {
				_userId = request->UserIdentifier;
				Global::NewConnectionRequested(request->UserIdentifier, this);
			}
			break;
		};
		case RequestType_NEW: {
			unsigned char buffer[UNCOMPRESSION_BUFFER_SIZE]{};
			int			  length = 0;
			Compression::DeCompressData((const unsigned char *)request->Message.data(), request->CompressedMsgLen, buffer, &length);
			std::stringstream ss;
			ss << buffer;
			nlohmann::json	newOrder = nlohmann::json::parse(ss);
			nlohmann::json &params	 = newOrder.at(JSON_PARAMS);

			int			id		 = newOrder.at(JSON_ID).get<int>();
			int			token	 = params.at(JSON_TOKEN).get<int>();
			float		price	 = std::stof(params.at(JSON_PRICE).get<std::string>());
			int			quantity = params.at(JSON_QUANTITY).get<int>();
			std::string client	 = params.at(JSON_CLIENT).get<std::string>();
			SideType	side	 = params.at(JSON_SIDE).get<SideType>();
			OrderType	type	 = params.at(JSON_ORDER_TYPE).get<OrderType>();
			LOG(INFO, "New Order {} {} {} {} {} {} {} ", id, token, price, quantity, client, side, type)
			break;
		}
		case RequestType_MODIFY: {
			unsigned char buffer[UNCOMPRESSION_BUFFER_SIZE]{};
			int			  length = 0;
			Compression::DeCompressData((const unsigned char *)request->Message.data(), request->CompressedMsgLen, buffer, &length);
			std::stringstream ss;
			ss << buffer;

			nlohmann::json	modifyOrder = nlohmann::json::parse(ss);
			nlohmann::json &params		= modifyOrder.at(JSON_PARAMS);

			int	  id		= modifyOrder.at(JSON_ID).get<int>();
			float price		= std::stof(params.at(JSON_PRICE).get<std::string>());
			int	  quantity	= params.at(JSON_QUANTITY).get<int>();
			long  order_id	= params.at(JSON_ORDER_ID).get<long>();
			int	  unique_id = params.at(JSON_UNIQUE_ID).get<int>();
			LOG(INFO, "Modify Order {} {} {} {} {}", id, price, quantity, order_id, unique_id)
			break;
		}
		case RequestType_CANCEL: {
			unsigned char buffer[UNCOMPRESSION_BUFFER_SIZE]{};
			int			  length = 0;
			Compression::DeCompressData((const unsigned char *)request->Message.data(), request->CompressedMsgLen, buffer, &length);
			std::stringstream ss;
			ss << buffer;

			nlohmann::json	cancelOrder = nlohmann::json::parse(ss);
			nlohmann::json &params		= cancelOrder.at(JSON_PARAMS);
			int				id			= cancelOrder.at(JSON_ID).get<int>();
			long			order_id	= params.at(JSON_ORDER_ID).get<long>();
			int				unique_id	= params.at(JSON_UNIQUE_ID).get<int>();
			LOG(INFO, "Cancel Order {} {} {}", id, order_id, unique_id)
			break;
		}
		default: break;
	}
	read();
}

void Connection::handle_write(const boost::system::error_code &error_, size_t size_) {
	if (error_) {
		LOG(WARNING, "{} {} {}", __FUNCTION__, error_.message(), size_)
	}
}
