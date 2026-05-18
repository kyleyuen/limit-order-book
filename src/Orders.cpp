#include <iostream> 
#include "Orders.h"

// Order Class method Implementation

// Constructor
Order::Order(unsigned int orderId, Side orderSide, Type orderType, unsigned int quantity, double price, unsigned int timestamp)
    : orderId(orderId), orderSide(orderSide), orderType(orderType), quantity(quantity), price(price), timestamp(timestamp) {
        remainingQuantity = quantity;
    }

// Getter Methods

unsigned int Order::getOrderId() const {
    return orderId;
}

unsigned int Order::getQuantity() const {
    return quantity;
}

bool Order::getIsFilled() const {
    return remainingQuantity == 0;
}

unsigned int Order::getRemainingQuantity() const {
    return remainingQuantity;
}

Side Order::getOrderedSide() const {
    return orderSide;
}

Type Order::getOrderedType() const {
    return orderType;
}

double Order::getPrice() const {
    return price;
}

unsigned int Order::getTimestamp() const {
    return timestamp;
}

// Update Remaining Quantity
void Order::updateRemainingQuantity(unsigned int filledQuantity) {
    if (filledQuantity > remainingQuantity) {
        return;
    }
    remainingQuantity -= filledQuantity;
}