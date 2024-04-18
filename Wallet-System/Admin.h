#pragma once
#include<iostream>
#include<string>
#include<set>
#include<vector>
#include "User.h"
using namespace std;

class Admin
{
	public:
		string name;
		string password;
		vector<User> users;
		set<string> uniqueUserName;
		bool isUnique(string name);
		//void addUser();
		vector<User> viewUsers();
		//void editUsers(User edited_User);
		//void viewTransactions();
		void suspendUsers(string suspendedUserName);
		void deleteUser(string deleted_User);
		void adjustUserBalance(string adjustedUserName, float value);
		int getIndex(string user_name);
};