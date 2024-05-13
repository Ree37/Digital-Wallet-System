#include "User.h"

#include <iostream>
#include <Algorithm>

#include "Container.h"
#include "Security/Utils.h"

using namespace std;



// Users functions
User::User(string username, string password) {
  if (username.empty() || password.empty()) {
    throw std::invalid_argument("Error: Username or password is empty.");
  }

  this->suspendedFlag = false;
  this->balancee = 0.0;
  this->username = username;
  this->password = password;
}

User::~User(){}

string User::getUsername() { return username; };

string User::getPassword() { return password; };

double User::getBalance() { return balancee; }

bool User::getSuspendedFlag() { return suspendedFlag; }

bool User::setUsername(string newUsername) {
  if (Container::Users.count(newUsername)) {
    return false;
  }

  username = newUsername;
  return true;
}

void User::setPassword(string newPassword) { password = newPassword; }

void User::setBalance(double amount) { 

    amount = round(amount * 100) / 100;
    if (amount < 0)
    {
        throw invalid_argument("Please Enter a Positive Number");
    }
    if (!isfinite(amount)) {
        throw invalid_argument("Balance Can't be infinity or nan");
    }

    this->balancee = amount;
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

void User::addMoney(double value) {
    value = round(value * 100) / 100;
    if (suspendedFlag) {
        throw invalid_argument("You are suspended!");
    }
    if (value <= 0 || !isfinite(value)) {
        throw invalid_argument("Enter valid amount!");
    }

    if (value + balancee > 100000)
    {
        throw invalid_argument("Maximum Balance Reached : 100,000");
    }
  
    setBalance(balancee + value);
  
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


void User::makeTransaction(string receiver, double amount) {
    Transaction* t = new Transaction();

    try {
        t->setSenderUsername(this->getUsername());
        if (suspendedFlag) {
            throw invalid_argument("You are suspended!");
        }
        t->setRecipientUsername(receiver);
        if (Container::getUser(receiver)->suspendedFlag) {
            throw invalid_argument(receiver + " is suspended!");
        }
        t->setAmount(amount);
        t->setIsPending(0);
        t->sendAmount();
        Container::addTransaction(t);
    }
    catch (const exception& e)
    {
        delete t;
        throw e;
    }

}
void User::makeRequest(string requested, double amount) {
    Transaction* t = new Transaction();
    try {
        t->setSenderUsername(requested);
        if (suspendedFlag) {
            throw invalid_argument("You are suspended!");
        }
        t->setRecipientUsername(this->getUsername());
        if (Container::getUser(requested)->suspendedFlag) {
            throw invalid_argument(requested + " is suspended!");
        }
        t->setAmount(amount);
        t->setIsPending(1);
        Container::addTransaction(t);
    }
    catch (const exception& e) {
        delete t;
        throw e;
    }
}

// Transaction functions
Transaction::Transaction() {
    this->sender = "";
    this->recipient = "";
    this->amountt = 0;
    this->dateTime = chrono::system_clock::now();
    this->isPending = 0;

}

Transaction::Transaction(string sender, string recipient, double amount) {
  this->sender = sender;
  this->recipient = recipient;
  setAmount(amount);
  this->dateTime = chrono::system_clock::now();
  this->isPending = 0;
}

void Transaction::setSenderUsername(string username) {
    Container::checkUserExist(username);
    this->sender = username;
}

void Transaction::setRecipientUsername(string username) {
    Container::checkUserExist(username);

    if (sender == username)
    {  
        throw invalid_argument("Can't send to self");   
    }
    this->recipient = username;
}

void Transaction::setAmount(double amount) {

    amount = round(amount * 100) / 100;

    if (amount <= 0 || !isfinite(amount))
    {
        throw invalid_argument("Please Enter a Positive Number");
    }

    this->amountt = amount;

}
bool Transaction::operator>(const Transaction& other) const {
    return other > *this;
}
void Transaction::setIsPending(int pending) { this->isPending = pending; }
void Transaction::setDateTime(chrono::system_clock::time_point date) { this->dateTime = date; }

string Transaction::getRecipientUserName() { return recipient; }
double Transaction::getAmount() { return amountt; }
int Transaction::getIsPending() { return isPending; }
chrono::system_clock::time_point Transaction::getDateTime() { return dateTime; }


void Transaction::checkSenderBalance() {
    if (Container::Users[sender]->getBalance() <= amountt)
    {
        throw invalid_argument("Insufficient balance");
    }
}
void Transaction::checkRecepientBalance() {
    if (Container::Users[recipient]->getBalance() + amountt > 100000)
    {
        throw invalid_argument("Maximum Balance Reached: 100,000.00 EGP");
    }
}


void Transaction::sendAmount() {

    checkSenderBalance();
    checkRecepientBalance();
    
    Container::Users[sender]->setBalance(Container::Users[sender]->getBalance() - amountt);
    Container::Users[recipient]->setBalance(Container::Users[recipient]->getBalance() + amountt);
   
}

string Transaction::getSenderUserName() { return sender; }



void Transaction::declineTransaction(){
    if (Container::Users[sender]->getSuspendedFlag()) {
        throw invalid_argument("You are suspended!");
    }
    this->isPending = 2;
    this->dateTime = chrono::system_clock::now();

}


void Transaction::acceptTransaction(){
    if (Container::Users[sender]->getSuspendedFlag()) {
        throw invalid_argument("You are suspended!");
    }
    if (Container::Users[recipient]->getSuspendedFlag()) {
        throw invalid_argument(recipient + " is suspended!");
    }
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
    os << "Sender: " << t.sender << " | Recepient: " << t.recipient << " | Amount: " << t.amountt << " EGP | Date: " << Utils::timePointToString(t.dateTime) << " | " << s;
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