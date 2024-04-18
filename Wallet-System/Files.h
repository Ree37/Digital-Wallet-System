#pragma once
#include <iostream>
#include <string>
#include"user.h"
#include <fstream>
using namespace std;
class Files
{
public:
	static void create();
	//void writeData(User inputUser);
	User* readData(string userName);
	static void writeUsersData(User* inputUser);
	static void writeTransactionsData(string sender, string recipient, float amount);
	static void updateBalance(User sender, User recipient, float amount);

};

