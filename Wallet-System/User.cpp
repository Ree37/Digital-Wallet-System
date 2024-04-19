#include "User.h"
#include "Security/Utils.h"
#include "Container.h"
#include "Security/bcrypt/BCryptDLL.h"
#include <iostream>

using namespace std;

//vector<User*> User::users;

//Users functions
User::User(string username, string password) {
	// Input Validation: check if username or password is empty
	if (username.empty() || password.empty()) {
		throw std::invalid_argument("Error: Username or password is empty.");
	}

    // Input Validation: check if password matches a good password policy
    if(!Utils::checkPasswordPolicy(password))
    {
        throw std::invalid_argument("Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long.");
    }
	//std::string hash = BCryptDLL::generateHash(password, 12);
    // store hash here

    //check if entered password matches the stored hash
    //std::cout << "checking right password: " << std::flush << BCryptDLL::validatePassword(password, hash) << std::endl;

    this->suspendedFlag = false;
    this->balance = 0.0;
    this->username = username;
    this->password = password;
}
float User::getBalance() { return balance; }

void User::setBalance(float value) {
	balance = value;
}

bool User::getSuspendedFlag()
{
	return suspendedFlag;
}
void User::setSuspendedFlag()
{
	suspendedFlag = !suspendedFlag;
};
string User::getUsername() { return username; };
string User::getPassword() { return password; };

vector<Transaction*> User::getSentTranactions() {
	return sentTransactions;
}
vector<Transaction*> User::getReceivedTranactions() {
	return receivedTransactions;
}

//Transaction functions
Transaction::Transaction(User sender, User recipient, float amount) {
	this->sender = &sender;
	this->recipient = &recipient;
	this->amount = amount;
}
void Transaction::addTransaction() {
	sender->getSentTranactions().push_back(this);
	recipient->getReceivedTranactions().push_back(this);
}
