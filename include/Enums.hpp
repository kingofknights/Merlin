#pragma once
enum OrderType : int {
	OrderType_LIMIT = 0,
	OrderType_MARKET,
	OrderType_IOC,
	OrderType_SPREAD
};

enum StrategyStatus : int {
	StrategyStatus_PENDING = 0,
	StrategyStatus_ACTIVE,
	StrategyStatus_APPLIED,
	StrategyStatus_INACTIVE,
	StrategyStatus_TERMINATED,
	StrategyStatus_WAITING,
	StrategyStatus_DISCONNECTED
};

enum OrderStatus : int {
	OrderStatus_NONE = 0,
	OrderStatus_PLACED,
	OrderStatus_NEW,
	OrderStatus_REPLACED,
	OrderStatus_CANCELLED,
	OrderStatus_NEW_REJECT,
	OrderStatus_REPLACE_REJECT,
	OrderStatus_CANCEL_REJECT,
	OrderStatus_PARTIAL_FILLED,
	OrderStatus_FILLED,
	OrderStatus_PENDING,
	OrderStatus_ADAPTOR_REJECT
};

enum OrderRequest : int {
	OrderRequest_NONE = 0,
	OrderRequest_NEW,
	OrderRequest_MODIFY,
	OrderRequest_CANCEL,
	OrderRequest_IOC,
	OrderRequest_CANCEL_ALL
};