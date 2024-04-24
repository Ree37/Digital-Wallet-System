#pragma once

#include <Unordered_Map>

#include "User.h"

class Container {
public:
	static unordered_map<string, User*> Users;
	static void addUser(User* user);
	static User* getUser(string userName);
	// void addTransaction(User *sender, User *recipient, float amount);
};
