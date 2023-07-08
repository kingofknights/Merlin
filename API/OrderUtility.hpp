//
// Created by VIKLOD on 15-06-2023.
//

#ifndef MERLIN_API_ORDER_UTILITY_HPP
#define MERLIN_API_ORDER_UTILITY_HPP

#include <memory>
#include <string>

namespace Lancelot {
	enum SideType : int;
}

enum OrderStatus : int;
struct OrderPacketT;
using OrderPacketPtrT = std::shared_ptr<OrderPacketT>;

namespace OrderUtility {
	bool GetIoc(const OrderPacketPtrT& orderPacket_);

	int GetPF(const OrderPacketPtrT& orderPacket_);

	int GetPrice(const OrderPacketPtrT& orderPacket_);

	int GetQuantity(const OrderPacketPtrT& orderPacket_);

	long GetOrderNumber(const OrderPacketPtrT& orderPacket_);

	char* GetClientCode(const OrderPacketPtrT& orderPacket_);

	char* GetAlgoID(const OrderPacketPtrT& orderPacket_);

	Lancelot::SideType GetSide(const OrderPacketPtrT& orderPacket_);

	int GetUniqueID(const OrderPacketPtrT& orderPacket_);

	int GetLastQuantity(const OrderPacketPtrT& orderPacket_);

	int GetLastPrice(const OrderPacketPtrT& orderPacket_);

	int GetTotalQuantity(const OrderPacketPtrT& orderPacket_);

	OrderStatus GetCurrentOrderStatus(const OrderPacketPtrT& orderPacket_);

	OrderStatus GetPreviousOrderStatus(const OrderPacketPtrT& orderPacket_);

	void SetIoc(const OrderPacketPtrT& orderPacket_, bool ioc_);

	void SetPF(const OrderPacketPtrT& orderPacket_, int pf_);

	void SetPrice(const OrderPacketPtrT& orderPacket_, int price_);

	void SetQuantity(const OrderPacketPtrT& orderPacket_, int quantity_);

	void SetOrderNumber(const OrderPacketPtrT& orderPacket_, long orderNumber_);

	void SetClientCode(const OrderPacketPtrT& orderPacket_, const std::string& client_);

	void SetAlgoID(const OrderPacketPtrT& orderPacket_, const std::string& algo_);

	void SetSide(const OrderPacketPtrT& orderPacket_, Lancelot::SideType side_);

	void SetLastQuantity(const OrderPacketPtrT& orderPacket_, int quantity_);

	void SetLastPrice(const OrderPacketPtrT& orderPacket_, int price_);

	void SetTotalQuantity(const OrderPacketPtrT& orderPacket_, int quantity_);

	void SetCurrentOrderStatus(const OrderPacketPtrT& orderPacket_, OrderStatus status_);

	void SetPreviousOrderStatus(const OrderPacketPtrT& orderPacket_, OrderStatus status_);

	void SetUniqueID(const OrderPacketPtrT& orderPacket_, int unique_);
}  // namespace OrderUtility

#endif	// MERLIN_API_ORDER_UTILITY_HPP
