#pragma once

enum Exchange { Exchange_NSE_FUTURE = 0, Exchange_NSE_EQUITY, Exchange_NSE_CURRENCY, Exchange_BSE_FUTURE, Exchange_BSE_CURRENCY, Exchange_END };

enum Instrument { Instrument_FUTURE = 0, Instrument_OPTION, Instrument_EQUITY, Instrument_OTHER };

enum OptionType {
	OptionType_NONE = 0,
	OptionType_CALL,
	OptionType_PUT,
};

enum SideType { Side_BUY = 0, Side_SELL };

enum OrderType { OrderType_LIMIT = 0, OrderType_MARKET, OrderType_IOC, OrderType_SPREAD };

enum StrategyStatus {
	StrategyStatus_PENDING = 0,
	StrategyStatus_ACTIVE,
	StrategyStatus_APPLIED,
	StrategyStatus_INACTIVE,
	StrategyStatus_TERMINATED,
	StrategyStatus_WAITING,
	StrategyStatus_DISCONNECTED
};

enum RequestType {
	RequestType_LOGIN = 0,
	// ORDER
	RequestType_NEW,
	RequestType_MODIFY,
	RequestType_CANCEL,
	// STRATEGY
	RequestType_APPLY,
	RequestType_SUBSCRIBE,
	RequestType_UNSUBSCRIBE,
	RequestType_SUBSCRIBE_APPLY,
};

enum ResponseType {
	// ORDER
	ResponseType_PLACED = 0,
	ResponseType_NEW,
	ResponseType_REPLACED,
	ResponseType_CANCELLED,
	ResponseType_NEW_REJECT,
	ResponseType_REPLACE_REJECT,
	ResponseType_CANCEL_REJECT,
	ResponseType_PARTIAL_FILLED,
	ResponseType_FILLED,

	// STRATEGY
	ResponseType_PENDING,
	ResponseType_SUBCRIBED,
	ResponseType_APPLIED,
	ResponseType_UNSUBSCRIBED,
	ResponseType_TERMINATED,
	ResponseType_UPDATES,
	ResponseType_EXCHANGE_DISCONNECT,

	// TRACKER
	ResponseType_TRACKER,
};

enum OrderStatus {
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

enum OrderRequest { OrderRequest_NONE = 0, OrderRequest_NEW, OrderRequest_MODIFY, OrderRequest_CANCEL, OrderRequest_IOC, OrderRequest_CANCEL_ALL };
