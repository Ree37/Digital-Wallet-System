#pragma once
#include <iostream>
#include <string>
#include"user.h"
#include <fstream>
using namespace std;
class Files
{
public:
	void create();
	void writeUsersData(User inputUser);
	void writeTransactionsData(User sender , User recipient , float amount);
	void editUserProfile();
	void updateBalance(User sender, User recipient, float amount);
	User readData(string userName);
};

