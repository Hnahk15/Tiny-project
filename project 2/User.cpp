#include "User.h"

int User::sNextId = 1;

User::User(const std::string& username, const std::string& password,
           const std::string& name)
    : mId(sNextId++), mUsername(username), mPassword(password), mName(name) {}

User::User(int id, const std::string& username, const std::string& password,
           const std::string& name)
    : mId(id), mUsername(username), mPassword(password), mName(name) {}

bool User::authenticate(const std::string& password) const {
    return mPassword == password;
}

void User::updateNextId(int id) {
    if (id >= sNextId) sNextId = id + 1;
}
