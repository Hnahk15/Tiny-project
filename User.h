#pragma once
#include <string>

// Abstract base for both Customer and Admin.
// Holds identity/auth data; role-specific behaviour is in subclasses.
class User {
protected:
    int         mId;
    std::string mUsername;
    std::string mPassword;   // Stored as plain text (acceptable for coursework scope)
    std::string mName;

    static int  sNextId;

public:
    // New user (ID auto-assigned)
    User(const std::string& username, const std::string& password,
         const std::string& name);

    // Reconstruct from file
    User(int id, const std::string& username, const std::string& password,
         const std::string& name);

    virtual ~User() = default;

    int         getId()       const { return mId; }
    std::string getUsername() const { return mUsername; }
    std::string getName()     const { return mName; }

    bool authenticate(const std::string& password) const;

    // Pure virtual — each subclass declares its role and serialises itself
    virtual std::string getRole()   const = 0;
    virtual std::string serialize() const = 0;

    static void updateNextId(int id);
};
