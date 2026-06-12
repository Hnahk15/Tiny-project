#pragma once
#include "Cart.h"
#include <vector>
#include <string>

class Order {
private:
    int                   mId;
    int                   mCustomerId;
    std::string           mDate;
    std::vector<CartItem> mItems;    // Snapshot of cart at checkout time
    double                mTotal;

    static int            sNextId;

public:
    Order() = default;

    // Create new order (ID auto-assigned)
    Order(int customerId, const std::string& date,
          const std::vector<CartItem>& items, double total);

    // Reconstruct from file
    Order(int id, int customerId, const std::string& date,
          const std::vector<CartItem>& items, double total);

    int                          getId()         const { return mId; }
    int                          getCustomerId() const { return mCustomerId; }
    std::string                  getDate()       const { return mDate; }
    double                       getTotal()      const { return mTotal; }
    const std::vector<CartItem>& getItems()      const { return mItems; }

    void display() const;

    std::string  serialize()   const;
    static Order deserialize(const std::string& line);
    static void  updateNextId(int id);
};
