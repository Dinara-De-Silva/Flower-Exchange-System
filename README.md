# Flower Exchange System

A multithreaded order-matching engine written in C++17. Orders are read from a CSV file, dispatched to per-instrument orderbooks running on dedicated threads, matched using price-time priority, and execution reports are written to an output CSV.

---

## Prerequisites

| Requirement | Version |
|---|---|
| C++ compiler | g++ / clang++ with **C++17** support, or MSVC 2019+ |
| Standard Library | `<thread>`, `<atomic>`, `<optional>` — all part of C++17 |

- **macOS / Linux**: g++ or clang++ (comes with Xcode Command Line Tools or `build-essential`)
- **Windows**: MinGW-w64 (g++) or Visual Studio 2019+ with the "Desktop development with C++" workload

---

## Building

All commands should be run from the **repository root**.

### macOS / Linux

```bash
g++ -std=c++17 \
    -IBackend/include \
    -IBackend/include/core \
    -IBackend/include/io \
    Backend/src/main.cpp \
    Backend/src/core/Order.cpp \
    Backend/src/core/BuyOrder.cpp \
    Backend/src/core/SellOrder.cpp \
    Backend/src/core/Orderbook.cpp \
    Backend/src/core/OrderbookSide.cpp \
    Backend/src/core/ExecutionReport.cpp \
    Backend/src/core/TimeService.cpp \
    Backend/src/io/CSVReader.cpp \
    Backend/src/io/CSVWriter.cpp \
    Backend/src/Exchange.cpp \
    -o Backend/build_test.exe
```

### Windows (MinGW-w64 / MSYS2)

Same command as above — MinGW-w64 supports `_WIN32` and routes to `localtime_s` automatically via the `#ifdef _WIN32` guard in `TimeService.cpp`.

```cmd
g++ -std=c++17 ^
    -IBackend/include ^
    -IBackend/include/core ^
    -IBackend/include/io ^
    Backend/src/main.cpp ^
    Backend/src/core/Order.cpp ^
    Backend/src/core/BuyOrder.cpp ^
    Backend/src/core/SellOrder.cpp ^
    Backend/src/core/Orderbook.cpp ^
    Backend/src/core/OrderbookSide.cpp ^
    Backend/src/core/ExecutionReport.cpp ^
    Backend/src/core/TimeService.cpp ^
    Backend/src/io/CSVReader.cpp ^
    Backend/src/io/CSVWriter.cpp ^
    Backend/src/Exchange.cpp ^
    -o Backend/build_test.exe
```

### Windows (Visual Studio)

Use the VS Code task defined in `.vscode/tasks.json` (**Terminal → Run Build Task**, or `Ctrl+Shift+B`), or add all the above source files to a Visual Studio project with the three include directories listed above.

---

## Running

```bash
./Backend/build_test.exe
```

On Windows:
```cmd
Backend\build_test.exe
```

---

## Input Format

Place orders in `Backend/data/input/orders.csv`:

```
clientOrderID,instrument,side,quantity,price
AA001,Rose,1,100,45.50
AA002,Rose,2,100,44.00
```

| Column | Type | Description |
|---|---|---|
| `clientOrderID` | string | Unique identifier for the order from the client |
| `instrument` | string | One of: `Rose`, `Lavendar`, `Lotus`, `Tulip`, `Orchid` |
| `side` | int | `1` = Buy, `2` = Sell |
| `quantity` | int | Must be a multiple of 10, between 10 and 10000 |
| `price` | double | Must be positive |

---

## Output Format

Execution reports are written to `Backend/data/output/execution_report.csv`:

```
OrderID,ClientOrderID,Instrument,Side,Price,Quantity,Status,ExecutedPrice,ExecutedQuantity,TransactionTime,Reason
```

| Status | Meaning |
|---|---|
| `new` | Order accepted and added to the book with no immediate match |
| `fill` | Order fully executed |
| `pfill` | Order partially executed; remainder stays in the book |
| `rejected` | Order failed validation (see `Reason` column) |

---

## Platform Notes

| Platform | Status | Notes |
|---|---|---|
| macOS | ✅ Fully supported | Uses `localtime_r` (POSIX) |
| Linux | ✅ Fully supported | Uses `localtime_r` (POSIX) |
| Windows | ✅ Supported | Uses `localtime_s` (MSVC/MinGW) via `#ifdef _WIN32` |

The only platform-specific call in the entire codebase is `localtime_r` / `localtime_s` in `TimeService.cpp`, which is handled with a preprocessor guard. All threading (`std::thread`, `std::atomic`, `std::mutex`) uses the C++17 standard library and is fully portable.
