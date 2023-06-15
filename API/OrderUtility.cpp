//
// Created by VIKLOD on 15-06-2023.
//

#include "OrderUtility.hpp"

#include "../include/Structure.hpp"

bool		OrderUtility::GetIoc(const OrderPacketPtrT& orderPacket_) { return orderPacket_->OrderDetails.Ioc; }
int			OrderUtility::GetPF(const OrderPacketPtrT& orderPacket_) { return orderPacket_->OrderDetails.PF; }
int			OrderUtility::GetPrice(const OrderPacketPtrT& orderPacket_) { return orderPacket_->OrderDetails.Price; }
int			OrderUtility::GetQuantity(const OrderPacketPtrT& orderPacket_) { return orderPacket_->OrderDetails.Quantity; }
long		OrderUtility::GetOrderNumber(const OrderPacketPtrT& orderPacket_) { return orderPacket_->OrderDetails.OrderNumber; }
char*		OrderUtility::GetClientCode(const OrderPacketPtrT& orderPacket_) { return orderPacket_->OrderDetails.ClientCode; }
char*		OrderUtility::GetAlgoID(const OrderPacketPtrT& orderPacket_) { return orderPacket_->OrderDetails.AlgoID; }
SideType	OrderUtility::GetSide(const OrderPacketPtrT& orderPacket_) { return orderPacket_->OrderDetails.Side; }
int			OrderUtility::GetLastQuantity(const OrderPacketPtrT& orderPacket_) { return orderPacket_->Position.LastQuantity; }
int			OrderUtility::GetLastPrice(const OrderPacketPtrT& orderPacket_) { return orderPacket_->Position.LastPrice; }
int			OrderUtility::GetUniqueID(const OrderPacketT& orderPacket_) { return orderPacket_.Internal.UniqueID; }
int			OrderUtility::GetTotalQuantity(const OrderPacketPtrT& orderPacket_) { return orderPacket_->Position.TotalQuantity; }
OrderStatus OrderUtility::GetCurrentOrderStatus(const OrderPacketPtrT& orderPacket_) { return orderPacket_->CurrentStatus; }
OrderStatus OrderUtility::GetPreviousOrderStatus(const OrderPacketPtrT& orderPacket_) { return orderPacket_->PreviousStatus; }

void OrderUtility::SetIoc(const OrderPacketPtrT& orderPacket_, bool ioc_) { orderPacket_->OrderDetails.Ioc = ioc_; }
void OrderUtility::SetPF(const OrderPacketPtrT& orderPacket_, int pf_) { orderPacket_->OrderDetails.PF = pf_; }
void OrderUtility::SetPrice(const OrderPacketPtrT& orderPacket_, int price_) { orderPacket_->OrderDetails.Price = price_; }
void OrderUtility::SetQuantity(const OrderPacketPtrT& orderPacket_, int quantity_) { orderPacket_->OrderDetails.Quantity = quantity_; }
void OrderUtility::SetOrderNumber(const OrderPacketPtrT& orderPacket_, long orderNumber_) { orderPacket_->OrderDetails.OrderNumber = orderNumber_; }
void OrderUtility::SetClientCode(const OrderPacketPtrT& orderPacket_, const std::string& client_) { std::memcpy(orderPacket_->OrderDetails.ClientCode, client_.data(), client_.length()); }
void OrderUtility::SetAlgoID(const OrderPacketPtrT& orderPacket_, const std::string& algo_) { std::memcpy(orderPacket_->OrderDetails.AlgoID, algo_.data(), algo_.length()); }
void OrderUtility::SetSide(const OrderPacketPtrT& orderPacket_, SideType side_) { orderPacket_->OrderDetails.Side = side_; }
void OrderUtility::SetLastQuantity(const OrderPacketPtrT& orderPacket_, int quantity_) { orderPacket_->Position.LastQuantity = quantity_; }
void OrderUtility::SetLastPrice(const OrderPacketPtrT& orderPacket_, int price_) { orderPacket_->Position.LastPrice = price_; }
void OrderUtility::SetTotalQuantity(const OrderPacketPtrT& orderPacket_, int quantity_) { orderPacket_->Position.TotalQuantity = quantity_; }
void OrderUtility::SetCurrentOrderStatus(const OrderPacketPtrT& orderPacket_, OrderStatus status_) { orderPacket_->CurrentStatus = status_; }
void OrderUtility::SetPreviousOrderStatus(const OrderPacketPtrT& orderPacket_, OrderStatus status_) { orderPacket_->PreviousStatus = status_; }
void OrderUtility::SetUniqueID(const OrderPacketPtrT& orderPacket_, int unique_) { orderPacket_->Internal.UniqueID = unique_; }
