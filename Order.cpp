#include "Order.h"
#include <sstream>
#include <cassert>
#include <vector>
#include <cstdio>

int Order::sNextId = 1;

Order::Order(int customerId, const std::string& date,
             const std::vector<CartItem>& items, double total)
    : mId(sNextId++), mCustomerId(customerId),
      mDate(date), mItems(items), mTotal(total) {}

Order::Order(int id, int customerId, const std::string& date,
             const std::vector<CartItem>& items, double total)
    : mId(id), mCustomerId(customerId),
      mDate(date), mItems(items), mTotal(total) {}

void Order::display() const {
    printf("\n  Order #%d  |  %s  |  Customer ID: %d\n",
           mId, mDate.c_str(), mCustomerId);
    printf("  %-28s %8s %5s %10s\n", "Product", "Price", "Qty", "Subtotal");
    printf("  ---------------------------------------------------\n");
    for (const auto& item : mItems) {
        printf("  %-28s %8.2f %5d %10.2f\n",
               item.product.name.c_str(),
               item.product.price,
               item.quantity,
               item.subtotal());
    }
    printf("  ---------------------------------------------------\n");
    printf("  %-40s %10.2f\n\n", "Total:", mTotal);
}

// Format: id|customerId|date|pid;name;category;price;qty,pid;....|total
std::string Order::serialize() const {
    std::ostringstream oss;
    oss << mId << "|" << mCustomerId << "|" << mDate << "|";
    for (size_t i = 0; i < mItems.size(); ++i) {
        if (i > 0) oss << ",";
        const auto& s = mItems[i].product;
        // Use ';' as sub-field separator inside each item entry
        oss << s.id << ";" << s.name << ";" << s.category << ";"
            << s.price << ";" << mItems[i].quantity;
    }
    oss << "|" << mTotal;
    return oss.str();
}

Order Order::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string tok;
    std::vector<std::string> f;
    while (std::getline(iss, tok, '|')) f.push_back(tok);
    assert(f.size() == 5);

    int         id         = std::stoi(f[0]);
    int         customerId = std::stoi(f[1]);
    std::string date       = f[2];
    double      total      = std::stod(f[4]);

    std::vector<CartItem> items;
    std::istringstream itemStream(f[3]);
    std::string entry;
    while (std::getline(itemStream, entry, ',')) {
        if (entry.empty()) continue;
        std::istringstream es(entry);
        std::string sf;
        std::vector<std::string> sub;
        while (std::getline(es, sf, ';')) sub.push_back(sf);
        if (sub.size() < 5) continue;
        ProductSnapshot snap;
        snap.id       = std::stoi(sub[0]);
        snap.name     = sub[1];
        snap.category = sub[2];
        snap.price    = std::stod(sub[3]);
        items.push_back({ snap, std::stoi(sub[4]) });
    }
    return Order(id, customerId, date, items, total);
}

void Order::updateNextId(int id) {
    if (id >= sNextId) sNextId = id + 1;
}
