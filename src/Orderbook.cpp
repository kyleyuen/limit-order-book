#include <iostream>
#include "Orders.h"
#include "Trades.h"
#include <unordered_map>
#include <deque>
#include <functional>
#include <map>
#include <vector>
#include <limits>
#include <iterator>
#include <algorithm>
#include "Orderbook.h"

// Orderbook Class method Implementation

// Manages the order book, processes incoming orders, and generates trades
std::vector<Trade> Orderbook::addOrder(const Order& order){
    std::vector<Trade> trades;
     
    // Add the order to the order book
    orders_.insert({order.getOrderId(), order});
    Order& storedOrder = orders_.at(order.getOrderId());

    // Match the order against the opposite side of the book
    if (storedOrder.getOrderedSide() == Side::buy) {
        trades = matchBuy(storedOrder);
    } else {
        trades = matchSell(storedOrder);
    }

    // If it's a limit order and not fully filled, add it to the book
    if (storedOrder.getOrderedType() == Type::limit && !storedOrder.getIsFilled()) {
        if (storedOrder.getOrderedSide() == Side::buy) {
            bids_[storedOrder.getPrice()].push_back(storedOrder.getOrderId());
        } else {
            asks_[storedOrder.getPrice()].push_back(storedOrder.getOrderId());
        }
    }

    return trades;
}

// Cancel an existing order by its ID
bool Orderbook::cancelOrder(unsigned int orderId){
    std::unordered_map<unsigned int, Order>::iterator it = orders_.find(orderId);
    if (it == orders_.end()){
        return false; // Order not found
    }
 
    //Order found, removing it from the order book 
    Order& order = it->second; 

    // Remove from the buy side 
    if(order.getOrderedSide() == Side::buy){
        std::deque<unsigned int>& queue = bids_[order.getPrice()];
        queue.erase(std::remove(queue.begin(), queue.end(), orderId), queue.end());
        if(queue.empty()){
            bids_.erase(order.getPrice());
        }
    } 

    // Remove from the sell side
    else {
        std::deque<unsigned int>& queue = asks_[order.getPrice()];
        queue.erase(std::remove(queue.begin(), queue.end(), orderId), queue.end());
        if(queue.empty()){
            asks_.erase(order.getPrice());
        }
    }

    orders_.erase(it);
    return true;
}

// Match a buy order against the sell side of the book --> Private function 
std::vector<Trade> Orderbook::matchBuy(Order& aggressiveOrder){
    std::vector<Trade> trades;
    

    std::map<double, std::deque<unsigned int>>::iterator it = asks_.begin();
    while(it != asks_.end() && aggressiveOrder.getRemainingQuantity() > 0){
        double askPrice = it->first; 
        std::deque<unsigned int>& queue = it->second;

        if (aggressiveOrder.getOrderedType() == Type::limit && aggressiveOrder.getPrice() < askPrice) {
            return trades; // No more matches possible
        }

        while(!queue.empty() && aggressiveOrder.getRemainingQuantity() > 0){
            unsigned int restingOrderId = queue.front();
            Order& restingOrder = orders_[restingOrderId];

            if(restingOrder.getIsFilled()){
                queue.pop_front();
                continue; 
            }

            unsigned int tradeQuantity = std::min(aggressiveOrder.getRemainingQuantity(), restingOrder.getRemainingQuantity());
            trades.emplace_back(restingOrderId, aggressiveOrder.getOrderId(), tradeQuantity, askPrice);

            aggressiveOrder.updateRemainingQuantity(tradeQuantity);
            restingOrder.updateRemainingQuantity(tradeQuantity);

            if (restingOrder.getIsFilled()) {
                queue.pop_front();
            }
        }
        if(queue.empty()){
            it = asks_.erase(it);
        } else {
            ++it;
        }
    }

    return trades;
}

// Match a sell order against the buy side of the book --> Private function
std::vector<Trade> Orderbook::matchSell(Order& aggressiveOrder){
    std::vector<Trade> trades;
    
    std::map<double, std::deque<unsigned int>, std::greater<double>>::iterator it = bids_.begin();
    while(it != bids_.end() && aggressiveOrder.getRemainingQuantity() > 0){
        double bidPrice = it->first; 
        std::deque<unsigned int>& queue = it->second;

        if (aggressiveOrder.getOrderedType() == Type::limit && aggressiveOrder.getPrice() > bidPrice) {
            return trades; // No more matches possible
        }

        while(!queue.empty() && aggressiveOrder.getRemainingQuantity() > 0){
            unsigned int restingOrderId = queue.front();
            Order& restingOrder = orders_[restingOrderId];

            if(restingOrder.getIsFilled()){
                queue.pop_front();
                continue; 
            }

            unsigned int tradeQuantity = std::min(aggressiveOrder.getRemainingQuantity(), restingOrder.getRemainingQuantity());
            trades.emplace_back(restingOrderId, aggressiveOrder.getOrderId(), tradeQuantity, bidPrice);

            aggressiveOrder.updateRemainingQuantity(tradeQuantity);
            restingOrder.updateRemainingQuantity(tradeQuantity);

            if (restingOrder.getIsFilled()) {
                queue.pop_front();
            }
        }
        if(queue.empty()){
            it = bids_.erase(it);
        } else {
            ++it;
        }
    }

    return trades;
}