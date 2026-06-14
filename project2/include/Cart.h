#pragma once
#include "Product.h"
#include <vector>

// One line in the shopping cart.
// Holds a snapshot (not a pointer) so the cart is unaffected
// by later edits or deletions of the live product.
struct CartItem {
    ProductSnapshot product;
    int             quantity;

    double subtotal() const { return product.price * quantity; }
};

class Cart {
private:
    std::vector<CartItem> mItems;

public:
    Cart() = default;

    // Returns false if product unavailable or qty invalid
    bool addItem(const Product& product, int qty);
    bool removeItem(int productId);
    bool updateQuantity(int productId, int newQty);

    bool   isEmpty()  const { return mItems.empty(); }
    double getTotal() const;
    int    size()     const { return (int)mItems.size(); }

    const std::vector<CartItem>& getItems() const { return mItems; }

    void display() const;
    void clear();
};
