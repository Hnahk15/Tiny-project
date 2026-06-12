#include "Admin.h"
#include <sstream>

Admin::Admin(const std::string& username, const std::string& password,
             const std::string& name)
    : User(username, password, name) {}

Admin::Admin(int id, const std::string& username, const std::string& password,
             const std::string& name)
    : User(id, username, password, name) {}

// Format: id|admin|username|password|name
std::string Admin::serialize() const {
    std::ostringstream oss;
    oss << mId << "|admin|" << mUsername << "|" << mPassword << "|" << mName;
    return oss.str();
}
