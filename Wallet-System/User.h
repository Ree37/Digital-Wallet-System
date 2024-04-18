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
		bool susbended_Flag;
		vector<Transaction> sentTransictions;
		vector<Transaction> receivedTransactions;
		static vector<User*> users;
public:
		User(string username, string password);
		User(string id);
		string getUsername();
		string getPassword();
		float getBalance();
		void setUsername(string newUsername);
		void setPassword(string newPassword);
		bool strongPassword();
		bool isUniqueUsername();
		void addUser();
		static void getUsers();

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

