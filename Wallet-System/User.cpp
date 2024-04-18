#include "User.h"
#include "Security/Utils.h"
#include <iostream>

User::User(std::string username, std::string password) {
    // Input Validation: check if username or password is empty
    if (username.empty() || password.empty()) {
        std::cerr << "Error: Username or password is empty." << std::endl;
        return;
    }

    // Input Validation: check if password matches a good password policy
    if(!Utils::checkPasswordPolicy(password))
    {
        std::cout << "Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long." << endl;
        return;
    }
    this->susbended_Flag = false;
    this->balance = 0.0;
    this->username = username;
    this->password = password;
}
float User::getBalance() { return balance; };
string User::getUsername() { return username; };
string User::getPassword() { return password; };
