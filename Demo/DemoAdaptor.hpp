#pragma once

#include "Adaptor.hpp"

class DemoAdaptor : public Adaptor {
public:
	DemoAdaptor(ThreadGroupT& threadGroup_);

	~DemoAdaptor() = default;

	void initialization(ThreadGroupT& threadContainer_) override;

	void forwardAssemble(const OrderPacketPtrT& order_) override;

	bool execute(const OrderPacketPtrT& order_, int price_, int quantity_, Lancelot::OrderRequest request_) override;
};

extern "C" AdaptorPtrT getDriver(ThreadGroupT& threadGroup_);
