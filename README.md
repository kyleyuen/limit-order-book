# Limit Order Book Simulator

This repository contains a small C++ limit order book simulator and benchmark. It implements a simplified trading engine that accepts synthetic buy and sell orders, matches them according to price/time priority, generates trades, and supports order cancellation.

## Project Structure

- `Makefile` - build script for compiling the project with `g++`.
- `include/`
  - `Orders.h` - order representation, including side, type, quantity, price, and remaining quantity.
  - `Trades.h` - trade representation containing resting order ID, aggressor order ID, quantity, and price.
  - `Orderbook.h` - order book class interface and internal data structures.
- `src/`
  - `main.cpp` - simulation driver that generates a mix of limit and market orders, cancels orders, and reports benchmark statistics.
  - `Orders.cpp` - implementation of the `Order` class.
  - `Orderbook.cpp` - matching engine implementation, including buy/sell matching and order cancellation.

## What it does

- Creates a synthetic workload of `100000` operations.
- Randomly generates buy and sell orders with a mix of market and limit orders.
- Matches aggressive orders against resting orders in the book.
- Supports partial fills and cancels existing orders.
- Measures and prints runtime, average latency, throughput, and sample trades.

## Build Instructions

The included `Makefile` is written for a Windows-style shell and uses `g++`.

To build the project, run:

```sh
make
```

This compiles the source files into object files and links them into `bin/app.exe`.

## Run

After building, run the executable:

```sh
./bin/app.exe
```

On macOS or Linux, you may need to adjust the `Makefile` paths to use forward slashes if necessary.

## Notes

- The project uses `std::map` and `std::deque` to maintain price levels and order queues.
- Limit orders are stored in the book when not fully matched.
- Market orders are matched immediately and do not persist in the book.
- Trades are created at the resting order price.
