#pragma once

#include <Unordered_Map>

#include "User.h"
#include "Admin.h"


class Admin;
class Container {
public:
	//static Admin adminn;
	static User* admin;
	static unordered_map<string, User*> Users;
    static vector<Transaction*> allTransactions;
    static unordered_map<string, vector<Transaction*>> userKeyTransactions;


public:
	static void addUser(User* user);
	static User* getUser(string userName);
	static void addTransaction(Transaction* t);
	static void checkUniqueUser(string username);
	static void checkUserExist(string username);
	static vector<Transaction *> getSentTransaction(string senderUserName);
    static vector<Transaction *> getRecipientTransaction(string recipientUserName);
    static vector<Transaction *> getAllUserTransaction(string UserName);
	~Container();
};
