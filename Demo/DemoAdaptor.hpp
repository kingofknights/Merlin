#pragma once

#include "Gawain.hpp"

class DemoAdaptor : public Lancelot::API::Adaptor {
  public:
    DemoAdaptor(Lancelot::ThreadGroupT& threadGroup_);

    ~DemoAdaptor() = default;

    void initialization(Lancelot::ThreadGroupT& threadContainer_) override;

    void forwardAssemble(const Lancelot::API::StockPacketPtrT& order_) override;

    bool execute(const Lancelot::API::StockPacketPtrT& order_, int price_, int quantity_, Lancelot::API::OrderRequest request_) override;
};

extern "C" Lancelot::API::AdaptorPtrT getObject(Lancelot::ThreadGroupT& threadGroup_);
