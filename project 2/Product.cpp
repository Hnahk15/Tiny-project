#include "Product.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <cstdio>

int Product::sNextId = 1;

Product::Product(const std::string& name, const std::string& category,
                 double price, int stock)
    : mId(sNextId++), mName(name), mCategory(category),
      mPrice(price), mStock(stock), mViewCount(0), mPurchaseCount(0) {}

Product::Product(int id, const std::string& name, const std::string& category,
                 double price, int stock, int viewCount, int purchaseCount)
    : mId(id), mName(name), mCategory(category),
      mPrice(price), mStock(stock), mViewCount(viewCount), mPurchaseCount(purchaseCount) {}

bool Product::reduceStock(int qty) {
    if (qty <= 0 || mStock < qty) return false;
    mStock -= qty;
    return true;
}

ProductSnapshot Product::snapshot() const {
    return { mId, mName, mCategory, mPrice };
}

void Product::displayBrief() const {
    printf("  %-4d %-28s %-15s %8.2f %6d\n",
           mId, mName.c_str(), mCategory.c_str(), mPrice, mStock);
}

void Product::display() const {
    printf("\n  +--------------------------------------------+\n");
    printf("  |  ID       : %-30d |\n", mId);
    printf("  |  Name     : %-30s |\n", mName.c_str());
    printf("  |  Category : %-30s |\n", mCategory.c_str());
    printf("  |  Price    : $%-29.2f |\n", mPrice);
    printf("  |  Stock    : %-30d |\n", mStock);
    printf("  |  Views    : %-30d |\n", mViewCount);
    printf("  |  Sold     : %-30d |\n", mPurchaseCount);
    printf("  +--------------------------------------------+\n\n");
}

// Format: id|name|category|price|stock|viewCount|purchaseCount
std::string Product::serialize() const {
    std::ostringstream oss;
    oss << mId << "|" << mName << "|" << mCategory << "|"
        << mPrice << "|" << mStock << "|" << mViewCount << "|" << mPurchaseCount;
    return oss.str();
}

Product Product::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string tok;
    std::vector<std::string> f;
    while (std::getline(iss, tok, '|')) f.push_back(tok);
    assert(f.size() == 7);
    return Product(std::stoi(f[0]), f[1], f[2],
                   std::stod(f[3]), std::stoi(f[4]),
                   std::stoi(f[5]), std::stoi(f[6]));
}

void Product::updateNextId(int id) {
    if (id >= sNextId) sNextId = id + 1;
}
