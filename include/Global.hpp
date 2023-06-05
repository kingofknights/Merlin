#pragma once

#include <memory>
#include <unordered_map>

class Connection;

namespace Global {

void NewConnectionRequested(uint64_t loginID_, const Connection* connection_);

void ConnectionClosed(uint64_t loginID_);
}  // namespace Global
