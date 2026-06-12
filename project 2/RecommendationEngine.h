#pragma once
#include "Product.h"
#include <map>
#include <utility>
#include <vector>
#include <string>

// Stateless engine — all methods are static.
// Implements the scoring formula from the specification:
//   score = viewCount * 0.2 + purchaseCount * 0.8 + categoryBonus
//
// categoryBonus = 1.0 if the product belongs to the user's most-interacted category,
//                 0.0 otherwise.
class RecommendationEngine {
public:
    // Apply the scoring formula to a single product
    static double computeScore(int viewCount, int purchaseCount, bool isInFavCategory);

    // Determine which category the user interacts with most
    static std::string findFavouriteCategory(
        const std::map<int, std::pair<int,int>>& interactions,
        const std::vector<Product>& catalog);

    // Return the top-N scored, available products for a user
    static std::vector<Product> getTopN(
        const std::map<int, std::pair<int,int>>& interactions,
        const std::vector<Product>& catalog,
        int N = 5);
};
