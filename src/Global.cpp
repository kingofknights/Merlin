#include "../include/Global.hpp"

#include "../include/Connection.hpp"
using ContainerPtrContainerT = std::unordered_map<uint64_t, const Connection*>;

static ContainerPtrContainerT ContainerPtrContainer;

void Global::NewConnectionRequested(uint64_t loginID_, const Connection* connection_) {
	ContainerPtrContainer.insert_or_assign(loginID_, connection_);
}

void Global::ConnectionClosed(uint64_t loginID_) {
	auto iterator = ContainerPtrContainer.find(loginID_);
	if (iterator != ContainerPtrContainer.end()) {
		if (iterator->second) {
			delete iterator->second;
		}
		ContainerPtrContainer.erase(iterator);
	}
}
