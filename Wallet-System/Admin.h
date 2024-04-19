#pragma once

#include<iostream>
#include<string>
#include<set>
#include<vector>
#include "User.h"

using namespace std;

class Admin {
private:
    string name;
    string password;
    vector <User> users;
    set <string> uniqueUserName;
public:
    Admin(string name, string password);

    bool isUnique(string name);

    void addUser();

    void viewUsers();

    void editUsers(User edited_User);

    void viewTransactions();

    void suspendUsers(string suspended_User_Name);

    void deleteUser(string deleted_User);

    void adjustUserBalance(string adjusted_user_name, int value);

    int getIndex(string user_name);
};