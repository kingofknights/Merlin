#include "DemoAdaptor.hpp"

#include "../include/Enums.hpp"
#include "../include/Structure.hpp"

DemoAdaptor::DemoAdaptor() {}

void DemoAdaptor::forwardAssemble(const OrderPacketPtrT &order_) {}

bool DemoAdaptor::execute(const OrderPacketPtrT &order_, int price_, int quantity_, OrderRequest request_) {
    return true;
}

void DemoAdaptor::initialization(ThreadGroupT &threadContainer_) {}

AdaptorPtrT getDriver(ThreadGroupT &threadGroup_) {
    return Adaptor::CreateInstance<DemoAdaptor>(threadGroup_);
}
