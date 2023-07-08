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

class Adaptor;
class SocketServer;
class Strategy;

using AdaptorPtrT	= std::shared_ptr<Adaptor>;
using SharedLibPtrT = std::unique_ptr<boost::dll::shared_library>;

namespace Lancelot {
	enum SideType : int;
}
#pragma pack(push, 1)

using RequestInPackT = struct RequestInPackT {
	short				  _totalSize;
	int					  _type;
	uint64_t			  _userIdentifier;
	int					  _encryptLength;
	std::array<char, 512> _encryptMessage;
};

#define CLIENT_CODE_LENGTH 10
#define ALGO_ID_LENGTH	   20

using PositionT = struct PositionT {
	int _lastQuantity;
	int _lastPrice;
	int _totalQuantity;
};

using OrderDetailsT = struct OrderDetailsT {
	char			   _clientCode[CLIENT_CODE_LENGTH];
	char			   _algoId[ALGO_ID_LENGTH];
	bool			   _ioc;
	int				   _pf;
	int				   _price;
	int				   _quantity;
	Lancelot::SideType _side;
	long			   _orderNumber;
};

class Strategy;
class Adaptor;
using StrategyPtrT = std::shared_ptr<Strategy>;
using AdaptorPtrT  = std::shared_ptr<Adaptor>;

using InternalT = struct InternalT {
	Lancelot::ResultSetPtrT _resultSetPtr;
	AdaptorPtrT				_adaptorPtr;
	StrategyPtrT			_strategyPtr;
	int						_uniqueId;
};

using OrderPacketT = struct OrderPacketT {
	InternalT	  _internal;
	OrderDetailsT _orderDetails;
	PositionT	  _position;

	OrderRequest _lastRequest;
	OrderStatus	 _currentStatus;
	OrderStatus	 _previousStatus;
};

using AdaptorConnectionT = struct AdaptorConnectionT {
	AdaptorPtrT	  _adaptorPtr;
	SharedLibPtrT _sharedLibPtr;
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
using OrderPacketPtrT		   = std::shared_ptr<OrderPacketT>;
using GlobalManualOrderPacketT = std::unordered_map<int, OrderPacketPtrT>;
