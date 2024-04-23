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
	void updateisHas2FA(User user);
	void updateTotpSecret(User user);
	void update2FA(User* user, bool isHas2FA, std::string secret);
	User* readData(string userName);
private:
	bool getline(istream& is, string& str, char delim);
};

