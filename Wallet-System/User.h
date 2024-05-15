#pragma once

#include <chrono>
#include <string>
#include <vector>

using namespace std;

class Transaction;

class User {
public:
  User(string username, string password);
  string getUsername();
  string getPassword();
  string getTotpSecret();
  double getBalance();
  bool getSuspendedFlag();
  bool getIsHas2FA();

  void setTotpSecret(string secret);
  void setIsHas2FA(bool flag);
  void setUsername(string newUsername);
  void setPassword(string newPassword);
  void setBalance(double amount);
  void setSuspendedFlag(bool flag);

  
  void addMoney(double value);

  vector<Transaction*> getAllTransactions();
  vector<Transaction*> getToRequests();
  vector<Transaction*> getFromRequests();

  vector<Transaction*> getReceivedTransactions();
  vector<Transaction*> getSentTransactions();
  void makeTransaction(string receiver, double amount);
  void makeRequest(string requested, double amount);

  friend std::ostream& operator<<(std::ostream& os, User& t);
  static bool compareByUsername(const User* a, const User* b);
  virtual ~User();
protected:
  string username;
  string password;
private:
  string totpSecret;
  bool isHas2FA = false;
  double balance;
  bool suspendedFlag;
};

class Transaction {
private:
  string sender;
  string recipient;
  double amountt;
  int isPending;
  chrono::system_clock::time_point dateTime;

public:
  bool operator>(const Transaction& other) const;
  Transaction();
  Transaction(string sender, string recipient, double amount);
  string getSenderUserName();
  string getRecipientUserName();
  double getAmount();
  int getIsPending();
  chrono::system_clock::time_point getDateTime();
  void setDateTime(chrono::system_clock::time_point date);
  void setIsPending(int pending);
  void setSenderUsername(string username);
  void setRecipientUsername(string username);
  void setAmount(double amount);

  void declineTransaction();
  void acceptTransaction();

  void checkSenderBalance();
  void checkRecepientBalance();
  
  void sendAmount();
  friend std::ostream& operator<<(std::ostream& os, const Transaction& t);

  static bool recentDate(Transaction* t1, Transaction* t2);

  static bool oldestDate(Transaction* t1, Transaction* t2);



};
