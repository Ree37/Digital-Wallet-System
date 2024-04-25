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
  bool isPending;
  chrono::system_clock::time_point dateTime;

public:
  Transaction(string sender, string recipient, float amount);
  string getSenderUserName();
  string getRecipientUserName();
  float getAmount();
  bool getIsPending();
  chrono::system_clock::time_point getDateTime();
  void setDateTime(chrono::system_clock::time_point date);
  void setIsPending(bool pending);
  bool checkSenderBalance(float amount);
  void sendAmount();
  void addTransaction();
};
