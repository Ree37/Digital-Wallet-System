#pragma once
#include<iostream>
#include<string>
#include<set>
#include<vector>
#include "User.h"
using namespace std;

class Admin
{
string admin_Name;
string admin_Password;
vector<User> users;
set<string> unique_Users_Name;
bool is_Unique(string name);
void add_User();
void view_Users();
void edit_User(User edited_User);
void view_Transactions();
void suspend_User(string suspended_User_Name );
void delete_User(string deleted_User);
void adjust_User_Balance(string adjusted_user_name,int value);
int get_Index(string user_name);
};