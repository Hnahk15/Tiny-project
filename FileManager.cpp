#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <vector>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

const std::string FileManager::PRODUCTS_FILE     = "data/products.txt";
const std::string FileManager::USERS_FILE        = "data/users.txt";
const std::string FileManager::ORDERS_FILE       = "data/orders.txt";
const std::string FileManager::INTERACTIONS_FILE = "data/interactions.txt";

void FileManager::ensureDataDir() {
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0755);
#endif
}

std::vector<std::string> FileManager::readLines(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream file(path);
    if (!file.is_open()) return lines;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) lines.push_back(line);
    return lines;
}

// -------- Products --------

void FileManager::saveProducts(const std::vector<Product>& products) {
    ensureDataDir();
    std::ofstream f(PRODUCTS_FILE);
    for (const auto& p : products) f << p.serialize() << "\n";
}

std::vector<Product> FileManager::loadProducts() {
    std::vector<Product> products;
    for (const auto& line : readLines(PRODUCTS_FILE)) {
        try {
            Product p = Product::deserialize(line);
            Product::updateNextId(p.getId());
            products.push_back(p);
        } catch (...) {}   // Skip malformed lines
    }
    return products;
}

// -------- Users --------

void FileManager::saveUsers(const std::vector<User*>& users) {
    ensureDataDir();
    std::ofstream f(USERS_FILE);
    for (const auto* u : users) f << u->serialize() << "\n";
}

std::vector<User*> FileManager::loadUsers() {
    std::vector<User*> users;
    for (const auto& line : readLines(USERS_FILE)) {
        try {
            // Format: id|role|username|password|name
            std::istringstream iss(line);
            std::string tok;
            std::vector<std::string> f;
            while (std::getline(iss, tok, '|')) f.push_back(tok);
            if (f.size() < 5) continue;

            int id = std::stoi(f[0]);
            User::updateNextId(id);

            if (f[1] == "admin")
                users.push_back(new Admin(id, f[2], f[3], f[4]));
            else
                users.push_back(new Customer(id, f[2], f[3], f[4]));
        } catch (...) {}
    }
    return users;
}

// -------- Orders --------

void FileManager::saveOrders(const std::vector<User*>& users) {
    ensureDataDir();
    std::ofstream f(ORDERS_FILE);
    for (const auto* u : users) {
        if (u->getRole() != "customer") continue;
        const Customer* c = dynamic_cast<const Customer*>(u);
        if (!c) continue;
        for (const auto& order : c->getOrders())
            f << order.serialize() << "\n";
    }
}

void FileManager::loadOrders(std::vector<User*>& users) {
    for (const auto& line : readLines(ORDERS_FILE)) {
        try {
            Order order = Order::deserialize(line);
            Order::updateNextId(order.getId());
            // Route order to the correct customer
            for (auto* u : users) {
                if (u->getRole() == "customer" && u->getId() == order.getCustomerId()) {
                    dynamic_cast<Customer*>(u)->addOrder(order);
                    break;
                }
            }
        } catch (...) {}
    }
}

// -------- Interactions --------

void FileManager::saveInteractions(const std::vector<User*>& users) {
    ensureDataDir();
    std::ofstream f(INTERACTIONS_FILE);
    for (const auto* u : users) {
        if (u->getRole() != "customer") continue;
        const Customer* c = dynamic_cast<const Customer*>(u);
        if (!c) continue;
        // Format: userId|productId|views|purchases
        for (const auto& [pid, counts] : c->getInteractions())
            f << u->getId() << "|" << pid << "|"
              << counts.first << "|" << counts.second << "\n";
    }
}

void FileManager::loadInteractions(std::vector<User*>& users) {
    for (const auto& line : readLines(INTERACTIONS_FILE)) {
        try {
            std::istringstream iss(line);
            std::string tok;
            std::vector<std::string> f;
            while (std::getline(iss, tok, '|')) f.push_back(tok);
            if (f.size() < 4) continue;

            int userId    = std::stoi(f[0]);
            int productId = std::stoi(f[1]);
            int views     = std::stoi(f[2]);
            int purchases = std::stoi(f[3]);

            for (auto* u : users) {
                if (u->getId() == userId && u->getRole() == "customer") {
                    dynamic_cast<Customer*>(u)->loadInteraction(productId, views, purchases);
                    break;
                }
            }
        } catch (...) {}
    }
}
