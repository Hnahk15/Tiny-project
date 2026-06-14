#pragma once
#include "User.h"

// Admin only needs identity + auth (no cart, no orders).
// Administrative operations live in ECommerceSystem to avoid coupling.
class Admin : public User {
public:
    Admin(const std::string& username, const std::string& password,
          const std::string& name);
    Admin(int id, const std::string& username, const std::string& password,
          const std::string& name);

    std::string getRole()   const override { return "admin"; }
    std::string serialize() const override;
};
