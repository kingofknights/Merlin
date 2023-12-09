#include <boost/asio.hpp>

#include "../include/Structure.hpp"

namespace MerlinShared {
int                     _globalJsonResponseUniqueId;
EventContainerT         _globalEventContainer;
StrategyContainerT      _globalStrategyContainer;
AdaptorContainerT       _globalAdaptorContainer;
ArthurMessageQueueT     _globalArthurMessageQueue;
boost::asio::io_context _globalStrategyThread;
}// namespace MerlinShared
