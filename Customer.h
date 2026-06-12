#pragma once
#include "User.h"
#include "Cart.h"
#include "Order.h"
#include <map>
#include <utility>
#include <vector>

class Customer : public User {
private:
    Cart               mCart;
    std::vector<Order> mOrderHistory;

    // Per-product interaction counts used by the recommendation engine.
    // Key: productId  |  Value: { viewCount, purchaseCount }
    std::map<int, std::pair<int,int>> mInteractions;

public:
    Customer(const std::string& username, const std::string& password,
             const std::string& name);
    Customer(int id, const std::string& username, const std::string& password,
             const std::string& name);

    // Cart access
    Cart&       getCart()       { return mCart; }
    const Cart& getCart() const { return mCart; }

    // Orders
    void                      addOrder(const Order& order);
    const std::vector<Order>& getOrders() const { return mOrderHistory; }
    void                      displayOrderHistory() const;

    // Interaction tracking
    void recordView(int productId);
    void recordPurchase(int productId, int qty);
    void loadInteraction(int productId, int views, int purchases); // File restore
    const std::map<int, std::pair<int,int>>& getInteractions() const { return mInteractions; }

    // Polymorphic interface
    std::string getRole()   const override { return "customer"; }
    std::string serialize() const override;
};
