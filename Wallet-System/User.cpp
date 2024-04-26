#include "User.h"

#include <iostream>
#include <regex>

#include "Container.h"
#include "Security/Utils.h"

using namespace std;

// Users functions
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

bool User::getSuspendedFlag() { return suspendedFlag; }

bool User::setUsername(string newUsername) {
  if (Container::Users.count(newUsername)) {
    return false;
  }

  username = newUsername;
  return true;
}

void User::setPassword(string newPassword) { password = newPassword; }

void User::setBalance(float amount) { balance = amount; }

void User::setSuspendedFlag(bool flag) { suspendedFlag = flag; }

string User::getTotpSecret() { return totpSecret; }

bool User::getIsHas2FA() { return isHas2FA; }

void User::setTotpSecret(string secret) { this->totpSecret = secret; }

void User::setIsHas2FA(bool flag) { this->isHas2FA = flag; }

bool User::addMoney(float value) {
  if (value > 0) {
    balance += value;
    return true;
  }
  return false;
}

bool User::strongPassword(string password) {
  regex pattern(
      "(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[@#$%^&+=])(?=\\S+$).{8,}");
  return (regex_match(password, pattern));
}

vector<Transaction*> User::getAllTransactions()
{
    return Container::getAllUserTransaction(this->username);
}


void User::makeTransaction(string receiver, float amount) {
    Transaction* t = new Transaction();

    t->setSenderUsername(this->getUsername());
    t->setRecipientUsername(receiver);
    t->setAmount(amount);
    t->setIsPending(false);
    t->sendAmount();

    Container::addTransaction(t);

}
void User::makeRequest(string requested, float amount) {
    Transaction* t = new Transaction();

    t->setSenderUsername(requested);
    t->setRecipientUsername(this->getUsername());
    t->setAmount(amount);
    t->setIsPending(true);

    Container::addTransaction(t);
}

// Transaction functions
Transaction::Transaction() {
    this->sender = "";
    this->recipient = "";
    this->amount = 0;
    this->dateTime = chrono::system_clock::now();
    this->isPending = false;

}

Transaction::Transaction(string sender, string recipient, float amount) {
  this->sender = sender;
  this->recipient = recipient;
  this->amount = amount;
  this->dateTime = chrono::system_clock::now();
  this->isPending = false;
}

void Transaction::setSenderUsername(string username) {
    checkUserExist(username);
    this->sender = username;
}

void Transaction::setRecipientUsername(string username) {
    checkUserExist(username);

    if (sender == username)
    {  
        throw invalid_argument("Can't send to self");   
    }
    this->recipient = username;
}

void Transaction::setAmount(float amount) {

    float constexpr inf = numeric_limits<float>::infinity();

    if (amount < 0)
    {
        throw invalid_argument("Please Enter a Positive Number");
    }
    if (amount == inf || amount == -inf) {
        throw invalid_argument("Amount Can't be infinity");
    }

}
void Transaction::setIsPending(bool pending) { this->isPending = pending; }
void Transaction::setDateTime(chrono::system_clock::time_point date) { this->dateTime = date; }

string Transaction::getRecipientUserName() { return recipient; }
float Transaction::getAmount() { return amount; }
bool Transaction::getIsPending() { return isPending; }
chrono::system_clock::time_point Transaction::getDateTime() { return dateTime; }


void Transaction::checkSenderBalance(float amount) {
    if (Container::Users[sender]->getBalance() <= amount)
    {
        throw invalid_argument("Insufficient balance");
    }
}

void Transaction::checkUserExist(string username) {

    if (!Container::Users.count(username))
    {
        throw invalid_argument(username + " doesn't exist");
    }

}

void Transaction::sendAmount() {

    checkSenderBalance(amount);

    Container::Users[sender]->setBalance(Container::Users[sender]->getBalance() - amount);
    Container::Users[recipient]->setBalance(Container::Users[recipient]->getBalance() + amount);
   
}

string Transaction::getSenderUserName() { return sender; }
