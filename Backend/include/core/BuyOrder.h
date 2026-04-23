#ifndef BUYORDER_H
#define BUYORDER_H

#include "Order.h"

// BuyOrder is no longer needed as a separate class.
// Side-aware ordering is handled by OrderbookSide's comparator.
// Kept as a type alias for backward compatibility.
using BuyOrder = Order;

#endif