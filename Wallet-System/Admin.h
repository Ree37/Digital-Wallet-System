#pragma once
#include<iostream>
#include<string>
#include<set>
#include<vector>
#include "User.h"
using namespace std;

class Admin
{
string name;
string password;
vector<User> users;
set<string> uniqueUserName;
bool isUnique(string name);
void addUser();
void viewUsers();
void editUsers(User edited_User);
void viewTransactions();
void suspendUsers(string suspended_User_Name );
void deleteUser(string deleted_User);
void adjustUserBalance(string adjusted_user_name,int value);
int getIndex(string user_name);
};