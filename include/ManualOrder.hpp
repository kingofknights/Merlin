//
// Created by kingofknights on 7/25/23.
//

#ifndef MERLIN_INCLUDE_MANUALORDER_HPP_
#define MERLIN_INCLUDE_MANUALORDER_HPP_
#pragma once

#include "Gawain.hpp"

class ManualOrder : public Lancelot::API::Strategy {
  public:
    explicit ManualOrder(int strategy_);
    ~ManualOrder() override = default;
    void paramEvent(const Lancelot::API::StrategyParamT& param_) override;
    void marketEvent(uint32_t token_) override;
    void orderEvent(int uniqueId_) override;
    void stopEvent() override;

    int  newOrder(uint32_t token_, Lancelot::Side side_, const std::string& client_, const std::string& algo_, int ioc_);
    void placeOrder(int uniqueId_, int price_, int quantity_, Lancelot::API::OrderRequest orderRequest_);

  private:
    using ManualOrderContainerT = std::unordered_map<int, Lancelot::API::StockPacketPtrT>;
    ManualOrderContainerT _manualOrderContainer;
};
#endif// MERLIN_INCLUDE_MANUALORDER_HPP_
