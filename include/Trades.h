#ifndef TRADES_H
#define TRADES_H

// Trade struct implementation 
// Created every time a trade happens between a resting order and an aggressor order

struct Trade{
    unsigned int restingOrderId;
    unsigned int aggressorOrderId;
    unsigned int quantity;
    double price;

    unsigned int getRestingOrderId() const { return restingOrderId; }
    unsigned int getAggressorOrderId() const { return aggressorOrderId; }
    unsigned int getQuantity() const { return quantity; }
    double getPrice() const { return price; }

    Trade(unsigned int restingOrderId, unsigned int aggressorOrderId, unsigned int quantity, double price)
        : restingOrderId(restingOrderId), aggressorOrderId(aggressorOrderId), quantity(quantity), price(price) {}
};

#endif  