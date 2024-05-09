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

User::~User(){}

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

void User::setBalance(float amount) { 

    if (amount < 0)
    {
        throw invalid_argument("Please Enter a Positive Number");
    }
    if (!isfinite(amount)) {
        throw invalid_argument("Balance Can't be infinity or nan");
    }

    this->balance = amount;
}

void User::setSuspendedFlag(bool flag) { suspendedFlag = flag; }

string User::getTotpSecret() { return totpSecret; }

bool User::getIsHas2FA() { return isHas2FA; }

void User::setTotpSecret(string secret) { this->totpSecret = secret; }

void User::setIsHas2FA(bool flag) { this->isHas2FA = flag; }

bool User::isUniqueUsername()
{
    auto uniqueUsername = Container::Users.find(this->username);
    return (uniqueUsername == Container::Users.end());
}

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

std::ostream& operator<<(std::ostream& os, User& u) {
 
    os << u.getUsername();
    return os;
}





vector<Transaction*> User::getToRequests()
{
    vector<Transaction*> requested;
    vector<Transaction*> all = Container::getSentTransaction(this->username);
    for (auto t : all) {
        if (t->getIsPending() == 1) {
            requested.push_back(t);
        }
    }
    auto comparator = Transaction::oldestDate;

    sort(requested.begin(), requested.end(), comparator);
    return requested;
}

vector<Transaction*> User::getFromRequests() {
    vector<Transaction*> requested;
    vector<Transaction*> all = Container::getRecipientTransaction(this->username);
    for (auto t : all) {
        if (t->getIsPending() == 1) {
            requested.push_back(t);
        }
    }
    auto comparator = Transaction::oldestDate;

    sort(requested.begin(), requested.end(), comparator);
    return requested;
}

vector<Transaction*> User::getAllTransactions()
{
    vector<Transaction*> resultAll;
    vector<Transaction*> all = Container::getAllUserTransaction(this->username);
    for (auto t : all) {
        if (t->getIsPending() != 1) {
            resultAll.push_back(t);
        }
    }

    auto comparator = Transaction::oldestDate;

    sort(resultAll.begin(), resultAll.end(), comparator);

    return resultAll;
}



vector<Transaction*> User::getReceivedTransactions()
{
    vector<Transaction*> recieved;
    vector<Transaction*> all = Container::getRecipientTransaction(this->username);
    for (auto t : all) {
        if (t->getIsPending() != 1) {
            recieved.push_back(t);
        }
    }
    auto comparator = Transaction::oldestDate;

    sort(recieved.begin(), recieved.end(), comparator);

    return recieved;
    
}

vector<Transaction*> User::getSentTransactions()
{
    vector<Transaction*> sent;
    vector<Transaction*> all = Container::getSentTransaction(this->username);
    for (auto t : all) {
        if (t->getIsPending() != 1) {
            sent.push_back(t);
        }
    }
    auto comparator = Transaction::oldestDate;

    sort(sent.begin(), sent.end(), comparator);

    return sent;
}


void User::makeTransaction(string receiver, float amount) {
    Transaction* t = new Transaction();

    try {
        t->setSenderUsername(this->getUsername());
        t->setRecipientUsername(receiver);
        t->setAmount(amount);
        t->setIsPending(0);
        t->sendAmount();
    }
    catch (exception e)
    {
        delete t;
        throw e;
    }

    Container::addTransaction(t);

}
void User::makeRequest(string requested, float amount) {
    Transaction* t = new Transaction();

    t->setSenderUsername(requested);
    t->setRecipientUsername(this->getUsername());
    t->setAmount(amount);
    t->setIsPending(1);

    Container::addTransaction(t);
}

// Transaction functions
Transaction::Transaction() {
    this->sender = "";
    this->recipient = "";
    this->amount = 0;
    this->dateTime = chrono::system_clock::now();
    this->isPending = 0;

}

Transaction::Transaction(string sender, string recipient, float amount) {
  this->sender = sender;
  this->recipient = recipient;
  this->amount = amount;
  this->dateTime = chrono::system_clock::now();
  this->isPending = 0;
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

    if (amount < 0)
    {
        throw invalid_argument("Please Enter a Positive Number");
    }
    if (!isfinite(amount)) {
        throw invalid_argument("Amount Can't be infinity or nan");
    }

    this->amount = amount;

}
bool Transaction::operator>(const Transaction& other) const {
    return other > *this;
}
void Transaction::setIsPending(int pending) { this->isPending = pending; }
void Transaction::setDateTime(chrono::system_clock::time_point date) { this->dateTime = date; }

string Transaction::getRecipientUserName() { return recipient; }
float Transaction::getAmount() { return amount; }
int Transaction::getIsPending() { return isPending; }
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



void Transaction::declineTransaction(){

    this->isPending = 2;
    this->dateTime = chrono::system_clock::now();

}


void Transaction::acceptTransaction(){
    sendAmount();
    this->isPending = 0;
    this->dateTime = chrono::system_clock::now();
}

std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    string s;
    switch (t.isPending) {
    case 0: s = "Completed"; break;
    case 1: s = "Pending"; break;
    case 2: s = "Declined";
    }
    os << "Sender: " << t.sender << " | Recepient: " << t.recipient << " | Amount: " << t.amount << " | Date: " << Utils::timePointToString(t.dateTime) << " | " << s;
    return os;
}

bool Transaction::oldestDate(Transaction* t1, Transaction* t2) {
    return t1->dateTime < t2->dateTime;
}

bool Transaction::recentDate(Transaction* t1, Transaction* t2) {
    return t1->dateTime > t2->dateTime;
}


 bool User::compareByUsername(const User* a, const User* b) {
    return a->username < b->username;
}