#include "DemoStrategy.hpp"

DemoStrategy::DemoStrategy(int pf_, const StrategyParameterT& strategyParameter_) : Strategy(pf_, strategyParameter_) { paramEvent(strategyParameter_); }

void DemoStrategy::paramEvent(const StrategyParameterT& strategyParameter_) {}

void DemoStrategy::marketEvent(int token_) {}

void DemoStrategy::orderEvent(int uniqueID_) {}

void DemoStrategy::stopEvent() {}

StrategyPtrT getDriver(int pf_, const StrategyParameterT& param_) { return std::make_shared<DemoStrategy>(pf_, param_); }
