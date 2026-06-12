#include "Cart.h"
#include <cstdio>
#include <string>

bool Cart::addItem(const Product& product, int qty) {
    if (qty <= 0 || !product.isAvailable()) return false;

    // Merge with existing entry if product already in cart
    for (auto& item : mItems) {
        if (item.product.id == product.getId()) {
            item.quantity += qty;
            return true;
        }
    }
    mItems.push_back({ product.snapshot(), qty });
    return true;
}

bool Cart::removeItem(int productId) {
    for (auto it = mItems.begin(); it != mItems.end(); ++it) {
        if (it->product.id == productId) {
            mItems.erase(it);
            return true;
        }
    }
    return false;
}

bool Cart::updateQuantity(int productId, int newQty) {
    if (newQty < 0) return false;
    if (newQty == 0) return removeItem(productId);
    for (auto& item : mItems) {
        if (item.product.id == productId) {
            item.quantity = newQty;
            return true;
        }
    }
    return false;
}

double Cart::getTotal() const {
    double total = 0.0;
    for (const auto& item : mItems) total += item.subtotal();
    return total;
}

void Cart::display() const {
    if (mItems.empty()) {
        printf("  Your cart is empty.\n");
        return;
    }
    printf("\n  %-4s %-28s %-15s %8s %5s %10s\n",
           "ID", "Name", "Category", "Price", "Qty", "Subtotal");
    printf("  -----------------------------------------------------------------------\n");
    for (const auto& item : mItems) {
        printf("  %-4d %-28s %-15s %8.2f %5d %10.2f\n",
               item.product.id,
               item.product.name.c_str(),
               item.product.category.c_str(),
               item.product.price,
               item.quantity,
               item.subtotal());
    }
    printf("  -----------------------------------------------------------------------\n");
    printf("  %-58s %10.2f\n\n", "TOTAL:", getTotal());
}

void Cart::clear() {
    mItems.clear();
}
