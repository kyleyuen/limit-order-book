#ifndef ORDERS_H
#define ORDERS_H

// Order class implementation
// Represents an individual order in the order book

enum class Side{
    buy,
    sell
};

enum class Type{
    market,
    limit
};

class Order{
public:

    // Compiler 
    Order() = default;
    Order(unsigned int orderId, Side orderSide, Type orderType, unsigned int quantity, double price, unsigned int timestamp);

    // Getter Method 
    unsigned int getOrderId() const;

    unsigned int getQuantity() const;

    bool getIsFilled() const;

    unsigned int getRemainingQuantity() const;

    Side getOrderedSide() const;

    Type getOrderedType() const;

    double getPrice() const;
    
    unsigned int getTimestamp() const; 

    // Update Remaining Quantity
    void updateRemainingQuantity(unsigned int filledQuanity);

private:
    unsigned int orderId;
    Side orderSide; 
    Type orderType; 
    unsigned int quantity; 
    double price; 
    unsigned int timestamp;
    unsigned int remainingQuantity;
};

#endif
