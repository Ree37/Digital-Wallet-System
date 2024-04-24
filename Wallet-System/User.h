#pragma once

#include <vector>
#include <string>

using namespace std;

class Transaction;
class Notification;

class User
{
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
	void setUsername(string newUsername);
	void setPassword(string newPassword);
	void setBalance(float amount);
	void setSuspendedFlag(bool flag);

	bool isUniqueUsername();
	bool addMoney(float value);
	bool strongPassword(string password);
	void addUser();
	vector<Transaction*> getTransactions();
	bool isSentTransaction(Transaction* transaction);
	void requestMoney(string username, float amount);

private:
	string username;
	string password;
	string totpSecret;
	bool isHas2FA = false;
	float balance;
	bool suspendedFlag;
	vector<Transaction*> transactions;
	vector<Notification*> notifications;

};

class Transaction {
private:
	User* sender;
	User* recipient;
	float amount;
public:
	Transaction(User sender, User recipient, float amount);
	User* getSender();
	User* getRecipient();
	bool checkSenderBalance(float amount);
	void sendAmount();
	void addTransaction();
};

class Notification {
	Notification(User receiver, float requestedMoney);
	User* notificationReceiver;
	float requestedMoney;
};
