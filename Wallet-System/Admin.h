 #pragma once
#include<iostream>
#include<string>
#include<set>
#include<vector>
#include "User.h"
#include "Container.h"

using namespace std;

class Admin
{
	public:
		bool isUnique(string name);
		void addUser(string userName,string userPassword);
		vector<User*> viewUsers();
		//void editUsers(User edited_User);
		//void viewTransactions();
		void setName(string name);
		void setPassword (string password);
		void setSuspendUsers(string suspendedUserName);
		void deleteUser(string deleted_User);
		void adjustUserBalance(string adjustedUserName, float value);
		void editUser(string name, string newName);
private:
	string name;
	string password;
};