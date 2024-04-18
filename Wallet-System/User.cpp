#include "User.h"
#include "Security/Utils.h"
#include <iostream>
using namespace std;

//Users functions
User::User(string username, string password) {
	// Input Validation: check if username or password is empty
	if (username.empty() || password.empty()) {
		cerr << "Error: Username or password is empty." << endl;
		return;
	}

	// Input Validation: check if password matches a good password policy
	if (!Utils::checkPasswordPolicy(password))
	{
		cout << "Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long." << endl;
		return;
	}
	this->susbended_Flag = false;
	this->balance = 0.0;
	this->username = username;
	this->password = password;
}
float User::getBalance() { return balance; };
string User::getUsername() { return username; };
string User::getPassword() { return password; };
void User::addUser() {
	users.push_back(this);
}
vector<User*> User::getUsers() {
	return users;
}

//Transaction functions
Transaction::Transaction(User sender, User recipient, float amount) {
	this->sender = &sender;
	this->recipient = &recipient;
	this->amount = amount;
}