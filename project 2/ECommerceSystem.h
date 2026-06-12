#pragma once
#include "Product.h"
#include "Customer.h"
#include "Admin.h"
#include "RecommendationEngine.h"
#include "FileManager.h"
#include <vector>
#include <string>

// Top-level controller.  Owns all products and users, coordinates
// menus, and delegates to domain classes for business logic.
class ECommerceSystem {
private:
    std::vector<Product> mProducts;
    std::vector<User*>   mUsers;
    User*                mCurrentUser;  // Non-owning pointer to active session

    // ---- Data helpers ----
    Product* findProduct(int id);
    User*    findUser(const std::string& username);
    bool     usernameExists(const std::string& username) const;
    void     seedDemoData();   // First-run initialisation
    void     saveAll()  const;

    // ---- Auth ----
    bool doLogin();
    void doRegister();

    // ---- Customer flows ----
    void customerMenu(Customer* c);
    void browseProducts(Customer* c);
    void manageCart(Customer* c);
    void checkout(Customer* c);
    void showRecommendations(Customer* c);
    void showOrderHistory(Customer* c);

    // ---- Admin flows ----
    void adminMenu(Admin* a);
    void adminListProducts();
    void adminAddProduct();
    void adminEditProduct();
    void adminDeleteProduct();
    void adminViewStats();

    // ---- UI utilities ----
    static void        printHeader(const std::string& title);
    static void        printSep();
    static int         readInt(const std::string& prompt);
    static double      readDouble(const std::string& prompt);
    static std::string readString(const std::string& prompt);
    static std::string currentDate();

public:
    ECommerceSystem();
    ~ECommerceSystem();

    void run();  // Main entry point
};
