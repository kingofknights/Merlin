#pragma once

#include "LancelotAPI.hpp"

class DemoAdaptor : public Lancelot::API::Adaptor {
public:
	DemoAdaptor(ThreadGroupT& threadGroup_);

	~DemoAdaptor() = default;

	void initialization(ThreadGroupT& threadContainer_) override;

	void forwardAssemble(const Lancelot::API::StockPacketPtrT& order_) override;

	bool execute(const Lancelot::API::StockPacketPtrT& order_, int price_, int quantity_, Lancelot::API::OrderRequest request_) override;
};

extern "C" Lancelot::API::AdaptorPtrT getObject(ThreadGroupT& threadGroup_);
