#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <vector>

#define ENTRY_FUNCTION_NAME "getDriver"
struct OrderPacketT;

using OrderPacketPtrT = std::shared_ptr<OrderPacketT>;
using ThreadPointerT  = std::unique_ptr<std::jthread>;
using ThreadGroupT	  = std::vector<ThreadPointerT>;

class Adaptor;
using AdaptorPtrT = std::shared_ptr<Adaptor>;

enum OrderRequest : int;
enum OrderStatus : int;

namespace Lancelot {
	enum ExchangeCode : int;
};

class Adaptor {
public:
	virtual ~Adaptor() = default;

	template <class Child>
	static AdaptorPtrT CreateInstance(ThreadGroupT& threadGroup_) {
		auto adaptor = std::make_unique<Child>();
		adaptor->initialization(threadGroup_);
		return adaptor;
	}

	virtual void initialization(ThreadGroupT& threadGroup_) = 0;

	virtual void forwardAssemble(const OrderPacketPtrT& order_) = 0;

	virtual bool execute(const OrderPacketPtrT& order_, int price_, int quantity_, OrderRequest request_) = 0;

	static void OrderResponse(const OrderPacketPtrT& order_, OrderStatus status_);

	static void OnDisconnection(Lancelot::ExchangeCode exchange_);

	static void OnConnection(Lancelot::ExchangeCode exchange_);
};
