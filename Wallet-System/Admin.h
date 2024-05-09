#pragma once
#include<iostream>
#include<string>
#include<set>
#include<vector>
#include "User.h"
#include "Container.h"

using namespace std;

class Admin : public User
{
public:

	    Admin(string username, string password);
		void addUser(string userName,string userPassword);
		vector<User*> viewUsers();
		//void editUsers(User edited_User);
		vector<Transaction*> viewSpecicTranactioBySenderName(string senderUserName);
		vector<Transaction*> viewSpecicTranactioByRecipientName(string recipientName);
		vector<Transaction*> viewAllUsersTransactions();

		void setSuspendUsers(string suspendedUserName);
		void deleteUser(string deleted_User);
		void adjustUserBalance(string adjustedUserName, float value);
		void editUser(string name, string newPwd);
		unordered_map<string, vector<Transaction*>>::iterator it;
};
