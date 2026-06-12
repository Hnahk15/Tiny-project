#pragma once
#include <string>

// Lightweight snapshot stored inside CartItem / OrderItem.
// Decoupled from the live catalog so cart/orders survive product edits or deletions.
struct ProductSnapshot {
    int         id;
    std::string name;
    std::string category;
    double      price;
};

class Product {
private:
    int         mId;
    std::string mName;
    std::string mCategory;
    double      mPrice;
    int         mStock;
    int         mViewCount;       // Global view count  (admin stats)
    int         mPurchaseCount;   // Global sales count (admin stats)

    static int  sNextId;          // Auto-increment ID pool

public:
    // New product (ID auto-assigned from pool)
    Product(const std::string& name, const std::string& category,
            double price, int stock);

    // Reconstruct from file (all fields explicitly provided)
    Product(int id, const std::string& name, const std::string& category,
            double price, int stock, int viewCount, int purchaseCount);

    // Getters
    int         getId()            const { return mId; }
    std::string getName()          const { return mName; }
    std::string getCategory()      const { return mCategory; }
    double      getPrice()         const { return mPrice; }
    int         getStock()         const { return mStock; }
    int         getViewCount()     const { return mViewCount; }
    int         getPurchaseCount() const { return mPurchaseCount; }

    // Setters (admin use)
    void setName(const std::string& name)    { mName = name; }
    void setCategory(const std::string& cat) { mCategory = cat; }
    void setPrice(double p)                  { mPrice = p; }
    void setStock(int s)                     { mStock = s; }

    // Business logic
    bool isAvailable()                    const { return mStock > 0; }
    bool reduceStock(int qty);                   // Returns false if stock insufficient
    void restoreStock(int qty)                   { mStock += qty; }
    void incrementView()                         { ++mViewCount; }
    void incrementPurchase(int qty = 1)          { mPurchaseCount += qty; }

    ProductSnapshot snapshot() const;            // Freeze current id/name/category/price

    // Display
    void display()      const;   // Full details card
    void displayBrief() const;   // Single table row

    // Serialization
    std::string    serialize()   const;
    static Product deserialize(const std::string& line);
    static void    updateNextId(int id);         // Advance pool after loading from file
};
