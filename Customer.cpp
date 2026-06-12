#include "Customer.h"
#include <sstream>
#include <cstdio>

Customer::Customer(const std::string& username, const std::string& password,
                   const std::string& name)
    : User(username, password, name) {}

Customer::Customer(int id, const std::string& username, const std::string& password,
                   const std::string& name)
    : User(id, username, password, name) {}

void Customer::addOrder(const Order& order) {
    mOrderHistory.push_back(order);
}

void Customer::displayOrderHistory() const {
    if (mOrderHistory.empty()) {
        printf("  No orders placed yet.\n");
        return;
    }
    for (const auto& order : mOrderHistory)
        order.display();
}

void Customer::recordView(int productId) {
    mInteractions[productId].first++;   // Increment view count
}

void Customer::recordPurchase(int productId, int qty) {
    mInteractions[productId].second += qty; // Accumulate purchase count
}

void Customer::loadInteraction(int productId, int views, int purchases) {
    mInteractions[productId] = { views, purchases };
}

// Format: id|customer|username|password|name
std::string Customer::serialize() const {
    std::ostringstream oss;
    oss << mId << "|customer|" << mUsername << "|" << mPassword << "|" << mName;
    return oss.str();
}
