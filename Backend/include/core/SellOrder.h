#ifndef SELLORDER_H
#define SELLORDER_H

#include "Order.h"

// SellOrder is no longer needed as a separate class.
// Side-aware ordering is handled by OrderbookSide's comparator.
// Kept as a type alias for backward compatibility.
using SellOrder = Order;

#endif