#include "User.h"

#include <iostream>
#include <regex>

#include "Security/Utils.h"
#include "Container.h"

using namespace std;

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

string User::getUsername()
{
	return username;
};

string User::getPassword()
{
	return password;
};

float User::getBalance()
{
	return balance;
}

bool User::getSuspendedFlag()
{
	return suspendedFlag;
}

bool User::setUsername(string newUsername)
{
	if (Container::Users.count(newUsername))
	{
		return false;
	}

	username = newUsername;
	return true;
}

void User::setPassword(string newPassword)
{
	
	password = newPassword;
	
}

void User::setBalance(float amount)
{
	balance = amount;
}

void User::setSuspendedFlag(bool flag)
{
	suspendedFlag = flag;
}

string User::getTotpSecret()
{
	return totpSecret;
}

bool User::getIsHas2FA()
{
	return isHas2FA;
}

void User::setTotpSecret(string secret)
{
	this->totpSecret = secret;
}

void User::setIsHas2FA(bool flag)
{
	this->isHas2FA = flag;
}

bool User::addMoney(float value) 
{
	if (value > 0) {
		balance += value;
		return true;
	}
	return false;
}

bool User::strongPassword(string password)
{
	regex pattern("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[@#$%^&+=])(?=\\S+$).{8,}");
	return (regex_match(password, pattern));

}

vector<Transaction*> User::getTransactions() 
{
	return transactions;
}

//Transaction functions
Transaction::Transaction(User sender, User recipient, float amount) 
{
	this->sender = &sender;
	this->recipient = &recipient;
	this->amount = amount;
}

void Transaction::addTransaction() 
{
	sender->getTransactions().push_back(this);
	recipient->getTransactions().push_back(this);
}

User* Transaction::getRecipient() 
{
	return recipient;
}

bool Transaction::checkSenderBalance(float amount)
{
	return (sender->getBalance() >= amount);
}

void Transaction::sendAmount()
{
	if (checkSenderBalance(amount)) {
		sender->setBalance(sender->getBalance() - amount);
		recipient->setBalance(recipient->getBalance() + amount);
	}
}
User* Transaction::getSender() 
{
	return sender;
}

//Notification functions
Notification::Notification(User receiver, float requestedMoney) 
{
	this->notificationReceiver = &receiver;
	this->requestedMoney = requestedMoney;
}

