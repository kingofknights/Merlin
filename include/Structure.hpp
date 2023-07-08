#pragma once

#include <Lancelot.hpp>
#include <array>
#include <boost/dll/shared_library.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "Enums.hpp"

#define JSON_PARAMS		   "params"
#define JSON_ID			   "_globalJsonResponseUniqueID"
#define JSON_TOKEN		   "token"
#define JSON_PRICE		   "price"
#define JSON_QUANTITY	   "quantity"
#define JSON_CLIENT		   "client"
#define JSON_SIDE		   "side"
#define JSON_ORDER_ID	   "order_id"
#define JSON_ARGUMENTS	   "arguments"
#define JSON_PF_NUMBER	   "pf"
#define JSON_UNIQUE_ID	   "unique_id"
#define JSON_STRATEGY_NAME "name"
#define JSON_ORDER_TYPE	   "type"
#define JSON_TIME		   "time"
#define JSON_FILL_QUANTITY "fill_quantity"
#define JSON_FILL_PRICE	   "fill_price"
#define JSON_REMAINING	   "remaining"
#define JSON_MESSAGE	   "message"

class Adaptor;
class SocketServer;
class Strategy;

using AdaptorPtrT	= std::shared_ptr<Adaptor>;
using SharedLibPtrT = std::unique_ptr<boost::dll::shared_library>;

#pragma pack(push, 1)

using RequestInPackT = struct RequestInPackT {
	short				  TotalSize;
	int					  Type;
	uint64_t			  UserIdentifier;
	int					  CompressedMsgLen;
	std::array<char, 512> Message;
};

#define CLIENT_CODE_LENGTH 10
#define ALGO_ID_LENGTH	   20

using PositionT = struct PositionT {
	int LastQuantity;
	int LastPrice;
	int TotalQuantity;
};

using OrderDetailsT = struct OrderDetailsT {
	bool	 Ioc;
	int		 PF;
	int		 Price;
	int		 Quantity;
	long	 OrderNumber;
	char	 ClientCode[CLIENT_CODE_LENGTH];
	char	 AlgoID[ALGO_ID_LENGTH];
	SideType Side;
};

class Strategy;
class Adaptor;
using StrategyPtrT = std::shared_ptr<Strategy>;
using AdaptorPtrT  = std::shared_ptr<Adaptor>;

using InternalT = struct InternalT {
	Lancelot::ResultSetPtrT ResultSetPtr;
	AdaptorPtrT				AdaptorPtr;
	StrategyPtrT			StrategyPtr;
	int						UniqueID;
};

using OrderPacketT = struct OrderPacketT {
	InternalT	  Internal;
	OrderDetailsT OrderDetails;
	PositionT	  Position;

	OrderRequest Request;
	OrderStatus	 CurrentStatus;
	OrderStatus	 PreviousStatus;
};

using AdaptorConnectionT = struct AdaptorConnectionT {
	AdaptorPtrT	  AdaptorPtr;
	SharedLibPtrT SharedLibPtr;
};

#pragma pack(pop)

using AdaptorContainerT		   = std::array<AdaptorConnectionT, Lancelot::Exchange_END>;
using ThreadPointerT		   = std::unique_ptr<std::jthread>;
using ThreadGroupT			   = std::vector<ThreadPointerT>;
using SocketServerPtrT		   = std::shared_ptr<SocketServer>;
using StrategyPtrT			   = std::shared_ptr<Strategy>;
using StrategyContainerT	   = std::unordered_map<int, StrategyPtrT>;
using EventContainerT		   = std::unordered_map<int, StrategyContainerT>;
using StrategyParameterT	   = std::unordered_map<std::string, std::string>;
using TokenToFutureTokenT	   = std::unordered_map<int, int>;
using OrderPacketPtrT		   = std::shared_ptr<OrderPacketT>;
using GlobalManualOrderPacketT = std::unordered_map<int, OrderPacketPtrT>;
