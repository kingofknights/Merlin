#pragma once

#include "Adaptor.hpp"

class DemoAdaptor : public Adaptor {
public:
    DemoAdaptor();

    ~DemoAdaptor() = default;

    void initialization(ThreadGroupT& threadContainer_) override;

    void forwardAssemble(const OrderPacketPtrT& order_) override;

    bool execute(const OrderPacketPtrT& order_, int price_, int quantity_, OrderRequest request_) override;
};

// extern __declspec(dllexport) AdaptorPtrT getDriver(ThreadGroupT& threadGroup_);
extern "C" void getDriver() {
    printf("%s\n", __FUNCTION__);
}
