//
// Created by VIKLOD on 15-06-2023.
//

#include "OrderUtility.hpp"

#include "../include/Structure.hpp"

bool			   OrderUtility::GetIoc(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_orderDetails._ioc; }
int				   OrderUtility::GetPF(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_orderDetails._pf; }
int				   OrderUtility::GetPrice(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_orderDetails._price; }
int				   OrderUtility::GetQuantity(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_orderDetails._quantity; }
long			   OrderUtility::GetOrderNumber(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_orderDetails._orderNumber; }
char*			   OrderUtility::GetClientCode(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_orderDetails._clientCode; }
char*			   OrderUtility::GetAlgoID(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_orderDetails._algoId; }
Lancelot::SideType OrderUtility::GetSide(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_orderDetails._side; }
int				   OrderUtility::GetLastQuantity(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_position._lastQuantity; }
int				   OrderUtility::GetLastPrice(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_position._lastPrice; }
int				   OrderUtility::GetUniqueID(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_internal._uniqueId; }
int				   OrderUtility::GetTotalQuantity(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_position._totalQuantity; }
OrderStatus		   OrderUtility::GetCurrentOrderStatus(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_currentStatus; }
OrderStatus		   OrderUtility::GetPreviousOrderStatus(const OrderPacketPtrT& orderPacket_) { return orderPacket_->_previousStatus; }

void OrderUtility::SetIoc(const OrderPacketPtrT& orderPacket_, bool ioc_) { orderPacket_->_orderDetails._ioc = ioc_; }
void OrderUtility::SetPF(const OrderPacketPtrT& orderPacket_, int pf_) { orderPacket_->_orderDetails._pf = pf_; }
void OrderUtility::SetPrice(const OrderPacketPtrT& orderPacket_, int price_) { orderPacket_->_orderDetails._price = price_; }
void OrderUtility::SetQuantity(const OrderPacketPtrT& orderPacket_, int quantity_) { orderPacket_->_orderDetails._quantity = quantity_; }
void OrderUtility::SetOrderNumber(const OrderPacketPtrT& orderPacket_, long orderNumber_) { orderPacket_->_orderDetails._orderNumber = orderNumber_; }
void OrderUtility::SetClientCode(const OrderPacketPtrT& orderPacket_, const std::string& client_) { std::memcpy(orderPacket_->_orderDetails._clientCode, client_.data(), client_.length()); }
void OrderUtility::SetAlgoID(const OrderPacketPtrT& orderPacket_, const std::string& algo_) { std::memcpy(orderPacket_->_orderDetails._algoId, algo_.data(), algo_.length()); }
void OrderUtility::SetSide(const OrderPacketPtrT& orderPacket_, Lancelot::SideType side_) { orderPacket_->_orderDetails._side = side_; }
void OrderUtility::SetLastQuantity(const OrderPacketPtrT& orderPacket_, int quantity_) { orderPacket_->_position._lastQuantity = quantity_; }
void OrderUtility::SetLastPrice(const OrderPacketPtrT& orderPacket_, int price_) { orderPacket_->_position._lastPrice = price_; }
void OrderUtility::SetTotalQuantity(const OrderPacketPtrT& orderPacket_, int quantity_) { orderPacket_->_position._totalQuantity = quantity_; }
void OrderUtility::SetCurrentOrderStatus(const OrderPacketPtrT& orderPacket_, OrderStatus status_) { orderPacket_->_currentStatus = status_; }
void OrderUtility::SetPreviousOrderStatus(const OrderPacketPtrT& orderPacket_, OrderStatus status_) { orderPacket_->_previousStatus = status_; }
void OrderUtility::SetUniqueID(const OrderPacketPtrT& orderPacket_, int unique_) { orderPacket_->_internal._uniqueId = unique_; }
