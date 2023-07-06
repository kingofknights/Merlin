#pragma once

#include "Strategy.hpp"

class DemoStrategy : public Strategy {
public:
    DemoStrategy(int pf_, const StrategyParameterT &strategyParameter_);
    void paramEvent(const StrategyParameterT &strategyParameter_) override;
    void marketEvent(int token_) override;
    void orderEvent(int uniqueID_) override;
    void stopEvent() override;
};

extern StrategyPtrT getDriver(int pf_, const StrategyParameterT &param_);
