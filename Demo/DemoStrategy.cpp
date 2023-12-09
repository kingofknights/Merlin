#include "DemoStrategy.hpp"

#include <Lancelot.hpp>
DemoStrategy::DemoStrategy(int strategy_, const Lancelot::API::StrategyParamT& strategyParameter_) : Strategy(strategy_) { paramEvent(strategyParameter_); }

DemoStrategy::~DemoStrategy() { LOG(ERROR, "{} {}", __FUNCTION__, getAddress()) }

void DemoStrategy::paramEvent(const Lancelot::API::StrategyParamT& strategyParameter_) { LOG(WARNING, "{} {}", __FUNCTION__, strategyParameter_) }

void DemoStrategy::marketEvent(uint32_t token_) {}

void DemoStrategy::orderEvent(int uniqueID_) {}

void DemoStrategy::stopEvent() { destroy(); }

Lancelot::API::StrategyPtrT getObject(int strategy_, const Lancelot::API::StrategyParamT& param_) { return std::make_shared<DemoStrategy>(strategy_, param_); }
