#pragma once
#include "Product.h"
#include "Customer.h"
#include "Admin.h"
#include "Order.h"
#include <vector>
#include <string>

// Handles all disk I/O for the system.
// Each entity type has a dedicated file under the data/ directory.
// File formats are pipe-delimited plain text for human readability.
class FileManager {
public:
    // Products: data/products.txt
    static void                 saveProducts(const std::vector<Product>& products);
    static std::vector<Product> loadProducts();

    // Users: data/users.txt   (caller takes ownership of raw pointers)
    static void              saveUsers(const std::vector<User*>& users);
    static std::vector<User*> loadUsers();

    // Orders: data/orders.txt  (stored flat; matched to customers on load)
    static void saveOrders(const std::vector<User*>& users);
    static void loadOrders(std::vector<User*>& users);

    // Interactions: data/interactions.txt
    static void saveInteractions(const std::vector<User*>& users);
    static void loadInteractions(std::vector<User*>& users);

private:
    static const std::string PRODUCTS_FILE;
    static const std::string USERS_FILE;
    static const std::string ORDERS_FILE;
    static const std::string INTERACTIONS_FILE;

    static void                     ensureDataDir();
    static std::vector<std::string> readLines(const std::string& path);
};
