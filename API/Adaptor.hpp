#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <thread>
#include <vector>

#define ENTRY_FUNCTION_NAME "getDriver"
struct OrderPacketT;

using OrderPacketPtrT = std::shared_ptr<OrderPacketT>;
using ThreadPointerT  = std::unique_ptr<std::jthread>;
using ThreadGroupT	  = std::vector<ThreadPointerT>;

class Adaptor;
using AdaptorPtrT = std::shared_ptr<Adaptor>;

enum OrderRequest;
enum OrderStatus;
enum Exchange;

class Adaptor {
public:
	Adaptor(boost::asio::io_context& ioContext_, ThreadGroupT& threadContainer_);

	virtual ~Adaptor() = default;

	template <class Child>
	static Adaptor CreateInstance(boost::asio::io_context& ioContext_, ThreadGroupT& threadContainer_) {
		return std::make_unique<Child>(ioContext_, threadContainer_);
	}

	virtual void prePackOrder(const OrderPacketPtrT& order_) = 0;

	virtual bool place(const OrderPacketPtrT& order_, OrderRequest request_) = 0;

protected:
	static void OrderResponse(const OrderPacketPtrT& order_, OrderStatus status_);

	static void OnDisconnection(Exchange exchange_);

	static void OnConnection(Exchange exchange_);
};
