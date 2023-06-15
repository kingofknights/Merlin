#pragma once

#include <memory>
#include <string>
#include <unordered_map>

struct OrderPacketT;

using OrderPacketPtrT	 = std::shared_ptr<OrderPacketT>;
using StrategyParameterT = std::unordered_map<std::string, std::string>;
enum SideType;

class Strategy : public std::enable_shared_from_this<Strategy> {
public:
	virtual void paramEvent(const StrategyParameterT &strategyParameter_) = 0;

	virtual void marketEvent(int token_) = 0;

	virtual void orderEvent(int uniqueID_) = 0;

	virtual void stopEvent() = 0;

	bool activated() const;
	void setActivated(bool activated_);

protected:
	Strategy(int pf_, const StrategyParameterT &strategyParameter_);

	void terminate();

	OrderPacketPtrT getOrderPacket(int token_, SideType side_, const std::string &client_, const std::string &algo_, int ioc_);

private:
	void registerForData(int token_);

	void registerStrategy();

protected:
	int _pf;

private:
	bool _activated;
};
