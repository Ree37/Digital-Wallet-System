#pragma once
#include <iostream>
#include <string>
#include"user.h"
#include <fstream>
using namespace std;
class Files
{
public:
<<<<<<< HEAD
	void create();
	void writeUsersData(User inputUser);
	void writeTransactionsData(User sender , User recipient , float amount);
	void editUserProfile();
	void updateBalance(User sender, User recipient, float amount);
	User readData(string userName);
=======
	static void create();
	//void writeData(User inputUser);
	static User* readData(string userName);
	static void writeUsersData(User* inputUser);
	static void writeTransactionsData(string sender, string recipient, float amount);
	static void updateBalance(User sender, User recipient, float amount);

>>>>>>> 947802d5d5cfb3ab424078f16ef4d586d82e4a0e
};

