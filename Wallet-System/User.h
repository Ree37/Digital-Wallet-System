#pragma once
#include <vector>
#include <string>
using namespace std;

class Transaction;

class User
{
private:
	string username;
	string password;
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
	float getBalance();
	bool getSuspendedFlag();
	void setUsername(string newUsername);
	void setPassword(string newPassword);
	void setSuspendedFlag();
	void addBalance(float value);
	bool strongPassword();
	bool isUniqueUsername();
	void addUser();
	vector<Transaction*> getTransactions();	
	bool isSentTransaction(Transaction *transaction);
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
	bool checkSenderBalance();
	void sendAmount();
	void addTransaction();
};

class Notification {
	Notification(User receiver, float requestedMoney);
	User* notificationReceiver;
	float requestedMoney;
	
};