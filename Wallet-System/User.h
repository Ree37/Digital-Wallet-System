#pragma once
#include <vector>
#include <string>
using namespace std;

class Transaction;
class Notification;

class User
{
private:
	string username;
	string password;
	string totpSecret;
	bool isHas2FA = false;
	float balance;
	bool suspendedFlag;
	vector<Transaction*> transactions;
	vector<Notification*> notifications;
	

public:
	//static vector<User*> users;
	User(string username, string password);
	/*User(string id);*/
	string getUsername();
	string getPassword();
	string getTotpSecret();
	void setTotpSecret(string secret);
	float getBalance();
	bool getSuspendedFlag();
	bool getIsHas2FA();
	void setIsHas2FA(bool flag);
	void setUsername(string newUsername);
	void setPassword(string newPassword);
	void setBalance(float amount);
	void setSuspendedFlag();
	void addMoney(float value, string password);
	bool strongPassword(string password);
	bool isUniqueUsername();
	void addUser();
	vector<Transaction*> getTransactions();	
	bool isSentTransaction(Transaction *transaction);
	void requestMoney(string username, float amount);
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