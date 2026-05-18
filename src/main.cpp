#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <chrono>
#include <limits>

#include "Orders.h"
#include "Trades.h"
#include "Orderbook.h"

int main() {
    std::cout << std::fixed << std::setprecision(2);

    constexpr int TOTAL_ORDERS = 100'000;
    constexpr int MARKET_PERCENT = 8;          // 8% market orders
    constexpr int CANCEL_EVERY = 12;           // cancel roughly every 12th operation
    constexpr unsigned int SEED = 2024;

    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<int> typeDist(0, 99);
    std::uniform_real_distribution<double> priceDist(95.0, 105.0);
    std::uniform_int_distribution<unsigned int> qtyDist(1, 1000);

    Orderbook book;
    std::vector<unsigned int> activeIds;
    std::vector<Trade> allTrades;
    unsigned int nextId = 1;
    int cancelsAttempted = 0, cancelsSucceeded = 0;

    std::cout << "Running simulation: " << TOTAL_ORDERS << " orders (limit/market mix, with cancels)\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < TOTAL_ORDERS; ++i) {
        // Possibly cancel an existing order instead of adding a new one
        if (i % CANCEL_EVERY == 0 && !activeIds.empty()) {
            std::uniform_int_distribution<size_t> idxDist(0, activeIds.size() - 1);
            size_t idx = idxDist(rng);
            unsigned int cancelId = activeIds[idx];
            ++cancelsAttempted;
            if (book.cancelOrder(cancelId))
                ++cancelsSucceeded;
            // Remove from active list (swap-and-pop)
            std::swap(activeIds[idx], activeIds.back());
            activeIds.pop_back();
            continue;   // skip adding a new order this iteration
        }

        Side side = (sideDist(rng) == 0) ? Side::buy : Side::sell;
        Type type = (typeDist(rng) < MARKET_PERCENT) ? Type::market : Type::limit;
        double price;
        if (type == Type::market) {
            price = (side == Side::buy) ? 0.0 : std::numeric_limits<double>::max();
        } else {
            price = priceDist(rng);
        }
        unsigned int qty = qtyDist(rng);

        Order order(nextId, side, type, qty, price, nextId);
        auto trades = book.addOrder(order);
        allTrades.insert(allTrades.end(), trades.begin(), trades.end());
        activeIds.push_back(nextId);
        ++nextId;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // -----------------------------------------------------------------
    // Benchmark summary
    // -----------------------------------------------------------------
    std::cout << "\n========== RESULTS ==========\n";
    std::cout << "Total operations   : " << TOTAL_ORDERS << " (adds + cancels)\n";
    std::cout << "Orders added       : " << (nextId - 1) << "\n";
    std::cout << "Cancels attempted  : " << cancelsAttempted << "\n";
    std::cout << "Cancels succeeded  : " << cancelsSucceeded << "\n";
    std::cout << "Trades generated   : " << allTrades.size() << "\n";
    std::cout << "Time               : " << durationUs << " microseconds\n";
    std::cout << "Average latency    : " << std::fixed << std::setprecision(3)
              << static_cast<double>(durationUs) / TOTAL_ORDERS << " us/op\n";
    std::cout << "Throughput         : " << (TOTAL_ORDERS * 1'000'000LL) / durationUs << " ops/sec\n";

    // Print first few trades as a sanity check
    std::cout << "\nSample of first 5 trades:\n";
    for (size_t i = 0; i < std::min(allTrades.size(), size_t(5)); ++i) {
        const auto& t = allTrades[i];
        std::cout << "  rest=" << t.restingOrderId
                  << " aggr=" << t.aggressorOrderId
                  << " qty=" << t.quantity
                  << " price=" << t.price << '\n';
    }

    std::cout << "\nSimulation complete.\n";
    return 0;
}