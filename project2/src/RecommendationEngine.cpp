#include "RecommendationEngine.h"
#include <algorithm>

double RecommendationEngine::computeScore(int views, int purchases, bool isInFavCategory) {
    // Spec formula: score = views*0.2 + purchases*0.8 + categoryBonus
    return views * 0.2 + purchases * 0.8 + (isInFavCategory ? 1.0 : 0.0);
}

std::string RecommendationEngine::findFavouriteCategory(
    const std::map<int, std::pair<int,int>>& interactions,
    const std::vector<Product>& catalog)
{
    // Tally total interaction weight (views + purchases) per category
    std::map<std::string, int> categoryWeight;
    for (const auto& [pid, counts] : interactions) {
        for (const auto& p : catalog) {
            if (p.getId() == pid) {
                categoryWeight[p.getCategory()] += counts.first + counts.second;
                break;
            }
        }
    }
    if (categoryWeight.empty()) return "";

    // Find the category with the highest combined weight
    auto maxIt = std::max_element(
        categoryWeight.begin(), categoryWeight.end(),
        [](const auto& a, const auto& b){ return a.second < b.second; });
    return maxIt->first;
}

std::vector<Product> RecommendationEngine::getTopN(
    const std::map<int, std::pair<int,int>>& interactions,
    const std::vector<Product>& catalog,
    int N)
{
    std::string favCategory = findFavouriteCategory(interactions, catalog);

    // Score every available product
    std::vector<std::pair<double, Product>> scored;
    for (const auto& p : catalog) {
        if (!p.isAvailable()) continue;

        int views = 0, purchases = 0;
        auto it = interactions.find(p.getId());
        if (it != interactions.end()) {
            views     = it->second.first;
            purchases = it->second.second;
        }

        bool inFav = (!favCategory.empty() && p.getCategory() == favCategory);
        scored.push_back({ computeScore(views, purchases, inFav), p });
    }

    // Sort descending by score
    std::sort(scored.begin(), scored.end(),
              [](const auto& a, const auto& b){ return a.first > b.first; });

    // Return top N products
    std::vector<Product> result;
    int count = std::min(N, (int)scored.size());
    result.reserve(count);
    for (int i = 0; i < count; ++i)
        result.push_back(scored[i].second);
    return result;
}
