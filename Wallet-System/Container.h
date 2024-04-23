#pragma once
#include <vector>
#include "User.h"
#include <Unordered_Map>
class Container
{
public:
	static vector<User*> users;
	static vector<Transaction> Transactions;
	static unordered_map<string, User> UsersData;
	void addUser(User* user);
	void addTransaction(User sender, User recipient, float amount);
	
};

