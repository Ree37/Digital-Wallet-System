#pragma once

#include <chrono>
#include <string>
#include <vector>

using namespace std;

class Transaction;
class Notification;

class User {
public:
  User(string username, string password);
  string getUsername();
  string getPassword();
  string getTotpSecret();
  float getBalance();
  bool getSuspendedFlag();
  bool getIsHas2FA();

  void setTotpSecret(string secret);
  void setIsHas2FA(bool flag);
  bool setUsername(string newUsername);
  void setPassword(string newPassword);
  void setBalance(float amount);
  void setSuspendedFlag(bool flag);

  bool isUniqueUsername();
  bool addMoney(float value);
  bool strongPassword(string password);
  bool isSentTransaction(Transaction *transaction);
  void requestMoney(string username, float amount);

  vector<Transaction*> getAllTransactions(bool recent = true);
  vector<Transaction*> getRequests(bool recent = true);
  vector<Transaction*> getFromRequests(bool recent = true);

  vector<Transaction*> getReceivedTransactions(bool recent = true);
  vector<Transaction*> getSentTransactions(bool recent = true);
  void makeTransaction(string receiver, float amount);
  void makeRequest(string requested, float amount);

  friend std::ostream& operator<<(std::ostream& os, User& t);

private:
  string username;
  string password;
  string totpSecret;
  bool isHas2FA = false;
  float balance;
  bool suspendedFlag;
};

class Transaction {
private:
  string sender;
  string recipient;
  float amount;
  int isPending;
  chrono::system_clock::time_point dateTime;

public:
  Transaction();
  Transaction(string sender, string recipient, float amount);
  string getSenderUserName();
  string getRecipientUserName();
  float getAmount();
  int getIsPending();
  chrono::system_clock::time_point getDateTime();
  void setDateTime(chrono::system_clock::time_point date);
  void setIsPending(int pending);
  void setSenderUsername(string username);
  void setRecipientUsername(string username);
  void setAmount(float amount);

  void declineTransaction();
  void acceptTransaction();

  void checkSenderBalance(float amount);
  void checkUserExist(string username);
  void sendAmount();
  void addTransaction();

  friend std::ostream& operator<<(std::ostream& os, const Transaction& t);

  static bool recentDate(Transaction* t1, Transaction* t2);

  static bool oldestDate(Transaction* t1, Transaction* t2);



};
