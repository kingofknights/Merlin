#pragma once

#include "Gawain.hpp"

class DemoStrategy : public Lancelot::API::Strategy {
  public:
    DemoStrategy(int pf_, const Lancelot::API::StrategyParamT& strategyParameter_);
    ~DemoStrategy();
    void paramEvent(const Lancelot::API::StrategyParamT& strategyParameter_) override;
    void marketEvent(uint32_t token_) override;
    void orderEvent(int uniqueID_) override;
    void stopEvent() override;
};

extern "C" Lancelot::API::StrategyPtrT getObject(int strategy_, const Lancelot::API::StrategyParamT& param_);