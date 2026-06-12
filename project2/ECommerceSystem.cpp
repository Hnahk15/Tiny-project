#include "ECommerceSystem.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <ctime>
#include <cstdio>

// =============================================================
//  Construction / Destruction
// =============================================================

ECommerceSystem::ECommerceSystem() : mCurrentUser(nullptr) {
    mProducts = FileManager::loadProducts();
    mUsers    = FileManager::loadUsers();
    FileManager::loadOrders(mUsers);
    FileManager::loadInteractions(mUsers);

    if (mProducts.empty() && mUsers.empty())
        seedDemoData();
}

ECommerceSystem::~ECommerceSystem() {
    for (auto* u : mUsers) delete u;
}

// =============================================================
//  Data helpers
// =============================================================

Product* ECommerceSystem::findProduct(int id) {
    for (auto& p : mProducts)
        if (p.getId() == id) return &p;
    return nullptr;
}

User* ECommerceSystem::findUser(const std::string& username) {
    for (auto* u : mUsers)
        if (u->getUsername() == username) return u;
    return nullptr;
}

bool ECommerceSystem::usernameExists(const std::string& username) const {
    for (const auto* u : mUsers)
        if (u->getUsername() == username) return true;
    return false;
}

void ECommerceSystem::seedDemoData() {
    // Default admin account
    mUsers.push_back(new Admin("admin", "admin123", "Administrator"));

    // Demo product catalogue
    mProducts.push_back(Product("Laptop Pro 15",        "Electronics", 1299.99, 15));
    mProducts.push_back(Product("Wireless Mouse",        "Electronics",   29.99, 50));
    mProducts.push_back(Product("Mechanical Keyboard",   "Electronics",   89.99, 25));
    mProducts.push_back(Product("USB-C Hub",             "Electronics",   39.99, 40));
    mProducts.push_back(Product("C++ Programming Book",  "Books",         49.99, 30));
    mProducts.push_back(Product("Data Structures Book",  "Books",         44.99, 20));
    mProducts.push_back(Product("Algorithms Textbook",   "Books",         59.99, 18));
    mProducts.push_back(Product("Office Chair",          "Furniture",    299.99, 10));
    mProducts.push_back(Product("Desk Lamp",             "Furniture",     45.99, 35));
    mProducts.push_back(Product("Monitor Stand",         "Furniture",     69.99, 22));

    saveAll();
    printf("  [System] First run detected — demo data loaded.\n");
    printf("  [System] Default admin login:  admin / admin123\n\n");
}

void ECommerceSystem::saveAll() const {
    FileManager::saveProducts(mProducts);
    FileManager::saveUsers(mUsers);
    FileManager::saveOrders(mUsers);
    FileManager::saveInteractions(mUsers);
}

// =============================================================
//  UI utilities
// =============================================================

void ECommerceSystem::printHeader(const std::string& title) {
    printf("\n  ====================================================\n");
    printf("     %s\n", title.c_str());
    printf("  ====================================================\n\n");
}

void ECommerceSystem::printSep() {
    printf("  ----------------------------------------------------\n");
}

int ECommerceSystem::readInt(const std::string& prompt) {
    int val;
    while (true) {
        printf("  %s", prompt.c_str());
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printf("  [!] Please enter a valid integer.\n");
    }
}

double ECommerceSystem::readDouble(const std::string& prompt) {
    double val;
    while (true) {
        printf("  %s", prompt.c_str());
        if (std::cin >> val && val >= 0.0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printf("  [!] Please enter a valid positive number.\n");
    }
}

std::string ECommerceSystem::readString(const std::string& prompt) {
    printf("  %s", prompt.c_str());
    std::string val;
    std::getline(std::cin, val);
    return val;
}

std::string ECommerceSystem::currentDate() {
    time_t now = time(nullptr);
    char   buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return std::string(buf);
}

// =============================================================
//  Main run loop
// =============================================================

void ECommerceSystem::run() {
    printHeader("Smart E-Commerce System");

    while (true) {
        printSep();
        printf("  1. Login\n");
        printf("  2. Register\n");
        printf("  3. Exit\n");
        printSep();

        switch (readInt("Choice: ")) {
            case 1:
                if (doLogin()) {
                    if (mCurrentUser->getRole() == "admin") {
                        Admin* a = dynamic_cast<Admin*>(mCurrentUser);
                        if (a) adminMenu(a);
                    } else {
                        Customer* c = dynamic_cast<Customer*>(mCurrentUser);
                        if (c) customerMenu(c);
                    }
                    mCurrentUser = nullptr;
                }
                break;
            case 2:
                doRegister();
                break;
            case 3:
                saveAll();
                printf("\n  Goodbye!\n\n");
                return;
            default:
                printf("  [!] Invalid choice.\n");
        }
    }
}

// =============================================================
//  Auth
// =============================================================

bool ECommerceSystem::doLogin() {
    printHeader("Login");
    std::string username = readString("Username : ");
    std::string password = readString("Password : ");

    User* user = findUser(username);
    if (!user || !user->authenticate(password)) {
        printf("\n  [!] Invalid username or password.\n");
        return false;
    }
    mCurrentUser = user;
    printf("\n  [+] Welcome back, %s!  (role: %s)\n",
           user->getName().c_str(), user->getRole().c_str());
    return true;
}

void ECommerceSystem::doRegister() {
    printHeader("Create Account");

    // Ensure username is unique and non-empty
    std::string username;
    while (true) {
        username = readString("Choose a username : ");
        if (username.empty())
            printf("  [!] Username cannot be empty.\n");
        else if (usernameExists(username))
            printf("  [!] Username already taken — please choose another.\n");
        else
            break;
    }

    std::string password = readString("Choose a password : ");
    if (password.empty()) { printf("  [!] Password cannot be empty.\n"); return; }

    std::string name = readString("Your full name    : ");
    if (name.empty()) name = username;

    mUsers.push_back(new Customer(username, password, name));
    saveAll();
    printf("\n  [+] Account created!  You can now log in as \"%s\".\n", username.c_str());
}

// =============================================================
//  Customer menu
// =============================================================

void ECommerceSystem::customerMenu(Customer* c) {
    while (true) {
        printHeader("Customer Menu  [" + c->getName() + "]");
        printf("  1. Browse Products\n");
        printf("  2. View / Manage Cart\n");
        printf("  3. Checkout\n");
        printf("  4. Get Recommendations\n");
        printf("  5. Order History\n");
        printf("  6. Logout\n");
        printSep();

        switch (readInt("Choice: ")) {
            case 1: browseProducts(c);      break;
            case 2: manageCart(c);          break;
            case 3: checkout(c);            break;
            case 4: showRecommendations(c); break;
            case 5: showOrderHistory(c);    break;
            case 6:
                printf("\n  [+] Logged out.\n");
                saveAll();
                return;
            default: printf("  [!] Invalid choice.\n");
        }
    }
}

void ECommerceSystem::browseProducts(Customer* c) {
    printHeader("Browse Products");

    if (mProducts.empty()) {
        printf("  No products in the catalogue.\n");
        return;
    }

    // Header row
    printf("  %-4s %-28s %-15s %8s %6s\n", "ID", "Name", "Category", "Price", "Stock");
    printSep();
    for (const auto& p : mProducts)
        if (p.isAvailable()) p.displayBrief();
    printSep();

    int id = readInt("Enter product ID to view details (0 to go back): ");
    if (id == 0) return;

    Product* p = findProduct(id);
    if (!p || !p->isAvailable()) {
        printf("  [!] Product not found or out of stock.\n");
        return;
    }

    // Viewing details counts as a user interaction
    p->incrementView();
    c->recordView(id);
    p->display();

    std::string ans = readString("Add to cart? (y/n): ");
    if (ans != "y" && ans != "Y") return;

    int qty = readInt("Quantity: ");
    if (qty <= 0) {
        printf("  [!] Quantity must be at least 1.\n");
        return;
    }
    if (qty > p->getStock()) {
        printf("  [!] Only %d unit(s) in stock.\n", p->getStock());
        return;
    }

    c->getCart().addItem(*p, qty);
    printf("  [+] Added %d x \"%s\" to cart.\n", qty, p->getName().c_str());
    saveAll();
}

void ECommerceSystem::manageCart(Customer* c) {
    while (true) {
        printHeader("Shopping Cart");
        c->getCart().display();

        printf("  1. Remove item\n");
        printf("  2. Update quantity\n");
        printf("  3. Back\n");
        printSep();

        int choice = readInt("Choice: ");
        if (choice == 3) return;

        if (c->getCart().isEmpty()) {
            printf("  [!] Cart is empty.\n");
            continue;
        }

        int pid = readInt("Enter product ID: ");

        if (choice == 1) {
            printf(c->getCart().removeItem(pid)
                       ? "  [+] Item removed.\n"
                       : "  [!] Item not found in cart.\n");

        } else if (choice == 2) {
            int newQty = readInt("New quantity (0 to remove): ");
            // Enforce stock ceiling if the product still exists
            Product* p = findProduct(pid);
            if (p && newQty > p->getStock()) {
                printf("  [!] Only %d unit(s) in stock.\n", p->getStock());
            } else {
                printf(c->getCart().updateQuantity(pid, newQty)
                           ? "  [+] Quantity updated.\n"
                           : "  [!] Item not found in cart.\n");
            }
        }
        saveAll();
    }
}

void ECommerceSystem::checkout(Customer* c) {
    printHeader("Checkout");

    if (c->getCart().isEmpty()) {
        printf("  [!] Your cart is empty.\n");
        return;
    }

    c->getCart().display();
    std::string confirm = readString("Confirm purchase? (y/n): ");
    if (confirm != "y" && confirm != "Y") {
        printf("  Purchase cancelled.\n");
        return;
    }

    // --- Validate stock for every item BEFORE any deduction ---
    for (const auto& item : c->getCart().getItems()) {
        Product* p = findProduct(item.product.id);
        if (!p) {
            printf("  [!] \"%s\" is no longer in the catalogue. Checkout aborted.\n",
                   item.product.name.c_str());
            return;
        }
        if (p->getStock() < item.quantity) {
            printf("  [!] Insufficient stock for \"%s\" (need %d, have %d). Checkout aborted.\n",
                   item.product.name.c_str(), item.quantity, p->getStock());
            return;
        }
    }

    // --- All checks passed — commit the transaction ---
    for (const auto& item : c->getCart().getItems()) {
        Product* p = findProduct(item.product.id);
        p->reduceStock(item.quantity);
        p->incrementPurchase(item.quantity);
        c->recordPurchase(item.product.id, item.quantity);
    }

    double total = c->getCart().getTotal();
    Order  order(c->getId(), currentDate(), c->getCart().getItems(), total);
    c->addOrder(order);
    c->getCart().clear();
    saveAll();

    printf("\n  [+] Order #%d placed successfully!\n", order.getId());
    printf("  [+] Total charged: $%.2f\n\n", total);
}

void ECommerceSystem::showRecommendations(Customer* c) {
    printHeader("Recommended For You");

    auto recs = RecommendationEngine::getTopN(c->getInteractions(), mProducts, 5);
    if (recs.empty()) {
        printf("  No recommendations yet — browse and view some products first!\n");
        return;
    }

    printf("  Based on your browsing and purchase history:\n\n");
    printf("  %-4s %-28s %-15s %8s %6s\n", "ID", "Name", "Category", "Price", "Stock");
    printSep();
    for (const auto& p : recs) p.displayBrief();
    printf("\n");
}

void ECommerceSystem::showOrderHistory(Customer* c) {
    printHeader("Order History");
    c->displayOrderHistory();
}

// =============================================================
//  Admin menu
// =============================================================

void ECommerceSystem::adminMenu(Admin* a) {
    while (true) {
        printHeader("Admin Panel  [" + a->getName() + "]");
        printf("  1. View All Products\n");
        printf("  2. Add Product\n");
        printf("  3. Edit Product\n");
        printf("  4. Delete Product\n");
        printf("  5. System Statistics\n");
        printf("  6. Logout\n");
        printSep();

        switch (readInt("Choice: ")) {
            case 1: adminListProducts();  break;
            case 2: adminAddProduct();    break;
            case 3: adminEditProduct();   break;
            case 4: adminDeleteProduct(); break;
            case 5: adminViewStats();     break;
            case 6:
                printf("\n  [+] Logged out.\n");
                saveAll();
                return;
            default: printf("  [!] Invalid choice.\n");
        }
    }
}

void ECommerceSystem::adminListProducts() {
    printHeader("All Products");
    if (mProducts.empty()) {
        printf("  No products.\n");
        return;
    }
    printf("  %-4s %-28s %-15s %8s %5s %6s %6s\n",
           "ID", "Name", "Category", "Price", "Stock", "Views", "Sales");
    printSep();
    for (const auto& p : mProducts) {
        printf("  %-4d %-28s %-15s %8.2f %5d %6d %6d\n",
               p.getId(), p.getName().c_str(), p.getCategory().c_str(),
               p.getPrice(), p.getStock(),
               p.getViewCount(), p.getPurchaseCount());
    }
    printf("\n");
}

void ECommerceSystem::adminAddProduct() {
    printHeader("Add New Product");

    std::string name     = readString("Product name : ");
    std::string category = readString("Category     : ");
    double      price    = readDouble("Price ($)    : ");
    int         stock    = readInt("Stock qty    : ");

    if (name.empty() || category.empty() || price <= 0.0 || stock < 0) {
        printf("  [!] Invalid product data.\n");
        return;
    }

    mProducts.emplace_back(name, category, price, stock);
    saveAll();
    printf("  [+] \"%s\" added with ID %d.\n",
           name.c_str(), mProducts.back().getId());
}

void ECommerceSystem::adminEditProduct() {
    adminListProducts();
    int id = readInt("Product ID to edit (0 to cancel): ");
    if (id == 0) return;

    Product* p = findProduct(id);
    if (!p) { printf("  [!] Product not found.\n"); return; }

    printHeader("Editing: " + p->getName());
    printf("  1. Name     (current: %s)\n",    p->getName().c_str());
    printf("  2. Category (current: %s)\n",    p->getCategory().c_str());
    printf("  3. Price    (current: $%.2f)\n", p->getPrice());
    printf("  4. Stock    (current: %d)\n",    p->getStock());
    printf("  5. Cancel\n");

    switch (readInt("Field: ")) {
        case 1: p->setName(readString("New name     : "));        break;
        case 2: p->setCategory(readString("New category : "));    break;
        case 3: p->setPrice(readDouble("New price ($): "));       break;
        case 4: p->setStock(readInt("New stock    : "));          break;
        case 5: return;
        default: printf("  [!] Invalid field.\n"); return;
    }
    saveAll();
    printf("  [+] Product updated.\n");
}

void ECommerceSystem::adminDeleteProduct() {
    adminListProducts();
    int id = readInt("Product ID to delete (0 to cancel): ");
    if (id == 0) return;

    auto it = std::find_if(mProducts.begin(), mProducts.end(),
                           [id](const Product& p){ return p.getId() == id; });
    if (it == mProducts.end()) {
        printf("  [!] Product not found.\n");
        return;
    }

    std::string ans = readString(
        "Delete \"" + it->getName() + "\"? This cannot be undone. (y/n): ");
    if (ans != "y" && ans != "Y") return;

    mProducts.erase(it);
    saveAll();
    printf("  [+] Product deleted.\n");
}

void ECommerceSystem::adminViewStats() {
    printHeader("System Statistics");

    int totalCustomers  = 0;
    int activeCustomers = 0;   // Customers with at least one order
    for (const auto* u : mUsers) {
        if (u->getRole() != "customer") continue;
        ++totalCustomers;
        const Customer* c = dynamic_cast<const Customer*>(u);
        if (c && !c->getOrders().empty()) ++activeCustomers;
    }

    // Find most-viewed and best-selling products
    const Product* mostViewed  = nullptr;
    const Product* bestSelling = nullptr;
    for (const auto& p : mProducts) {
        if (!mostViewed  || p.getViewCount()     > mostViewed->getViewCount())
            mostViewed  = &p;
        if (!bestSelling || p.getPurchaseCount() > bestSelling->getPurchaseCount())
            bestSelling = &p;
    }

    printf("  Total products    : %d\n",   (int)mProducts.size());
    printf("  Total users       : %d\n",   (int)mUsers.size());
    printf("  Customers         : %d\n",   totalCustomers);
    printf("  Active customers  : %d\n",   activeCustomers);

    if (mostViewed)
        printf("  Most viewed       : %s  (%d views)\n",
               mostViewed->getName().c_str(), mostViewed->getViewCount());

    if (bestSelling)
        printf("  Best selling      : %s  (%d sold)\n",
               bestSelling->getName().c_str(), bestSelling->getPurchaseCount());

    printf("\n");
}
