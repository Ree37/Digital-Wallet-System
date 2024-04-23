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
	

public:
	User(string username, string password);
	User(string id);
	string getUsername();
	string getPassword();
	float getBalance();
	bool getSuspendedFlag();
	void setUsername(string newUsername);
	void setPassword(string newPassword);
	void setSuspendedFlag();
	void setBalance(float value);
	bool strongPassword();
	bool isUniqueUsername();
	void addUser();
	static vector<User*> getUsers();
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

