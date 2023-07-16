#include "DemoStrategy.hpp"

DemoStrategy::DemoStrategy(int strategy_, const Lancelot::API::StrategyParamT& strategyParameter_) : Strategy(strategy_) { paramEvent(strategyParameter_); }

void DemoStrategy::paramEvent(const Lancelot::API::StrategyParamT& strategyParameter_) {}

void DemoStrategy::marketEvent(int token_) {}

void DemoStrategy::orderEvent(int uniqueID_) {}

void DemoStrategy::stopEvent() {}

Lancelot::API::StrategyPtrT getDriver(int strategy_, const Lancelot::API::StrategyParamT& param_) { return std::make_shared<DemoStrategy>(strategy_, param_); }
