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
	static vector<User*> users;
	vector<Transaction> sentTransictions;
	vector<Transaction> receivedTransactions;
public:
	User(string username, string password);
	string getUsername();
	string getPassword();
	float getBalance();
	void setUsername(string newUsername);
	void setPassword(string newPassword);
	bool strongPassword();
	bool isUniqueUsername();
	void addUser();

};


class Transaction {
private:
	User *sender;
	User *recipient;
	float amount;
public:
	Transaction(User sender, User recipient, float amount);
	bool checkSenderBalance();
	void sendAmount();
	void addToSentTransactions();
	void addToReceivedTransactions();
};

