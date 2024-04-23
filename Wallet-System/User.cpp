#include "User.h"
#include "Security/Utils.h"
#include "Container.h"
#include <iostream>

using namespace std;

//vector<User*> User::users;

//Users functions
User::User(string username, string password) {
	if (username.empty() || password.empty()) {
		throw std::invalid_argument("Error: Username or password is empty.");
	}

    
    this->suspendedFlag = false;
    this->balance = 0.0;
    this->username = username;
    this->password = password;
}
string User::getUsername() { return username; };

string User::getPassword() { return password; };

float User::getBalance() { return balance; }

bool User::getSuspendedFlag()
{
	return suspendedFlag;
}

void User::setUsername(string newUsername)
{
	username = newUsername;
}

void User::setPassword(string newPassword)
{
	password = newPassword;
}

void User::setSuspendedFlag()
{
	suspendedFlag = !suspendedFlag;
};

void User::addBalance(float value) {
	balance += value ;
}

vector<Transaction*> User::getTransactions() {
	return transactions;
}
bool User::isUniqueUsername() {
	for (int i = 0; i < Container::users.capacity(); i++)
	{
		if (this->username == Container::users[i]->username) {
			return false;
		}
	}
	return true;
}
bool User::isSentTransaction(Transaction *transaction) {
	if (transaction->getSender() == this) {
		return true;
	}
	return false;
}

//Transaction functions
Transaction::Transaction(User sender, User recipient, float amount) {
	this->sender = &sender;
	this->recipient = &recipient;
	this->amount = amount;
}
void Transaction::addTransaction() {
	sender->getTransactions().push_back(this);
	recipient->getTransactions().push_back(this);
}
User* Transaction::getRecipient() {
	return recipient;
}
User* Transaction::getSender() {
	return sender;
}

//Notification functions
Notification::Notification(User receiver, float requestedMoney) {
	this->notificationReceiver = &receiver;
	this->requestedMoney = requestedMoney;
}


