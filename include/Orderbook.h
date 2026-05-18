#include <iostream>
#include "Orders.h"
#include "Trades.h"
#include <unordered_map>
#include <deque>
#include <functional>
#include <map>
#include <vector>

#ifndef ORDERBOOK_H
#define ORDERBOOK_H

// Orderbook Class method Implementation
// Manages the order book, processes incoming orders, and generates trades

class Orderbook {
public:
    std::vector<Trade> addOrder(const Order& order);
    bool cancelOrder(unsigned int orderId);

private:
    std::map<double, std::deque<unsigned int>, std::greater<double>> bids_;
    std::map<double, std::deque<unsigned int>> asks_;
    std::unordered_map<unsigned int, Order> orders_; 

    std::vector<Trade> matchBuy(Order& aggressiveOrder);
    std::vector<Trade> matchSell(Order& aggressiveOrder);
};

#endif