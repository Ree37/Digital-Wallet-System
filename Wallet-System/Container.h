#pragma once

#include <Unordered_Map>

#include "User.h"

class Container {
public:
	static unordered_map<string, User*> Users;
    static vector<Transaction*> allTransactions;
    static unordered_map<string, vector<Transaction*>> userKeyTransactions;
	static void addUser(User* user);
	static User* getUser(string userName);
	static void addTransaction(Transaction* t);
	static vector<Transaction *> getSentTransaction(string senderUserName);
    static vector<Transaction *> getRecipientTransaction(string recipientUserName);
    static vector<Transaction *> getAllUserTransaction(string UserName);
};
