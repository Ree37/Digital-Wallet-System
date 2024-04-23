#pragma once
#include <iostream>
#include"Container.h"
#include <string>
#include"user.h"
#include <fstream>
using namespace std;
class Files
{
private:
	void create();
<<<<<<< HEAD
public:
	void writeUsersDataFromContainerToFiles(vector <User*> users);
	void writeTransactionsDataFromContainerToFiles(vector<Transaction>Transactions);
	stringstream getLineFromData(ifstream &data);
	string getCellFromLine(stringstream &record);
	User* readDataFromCsvFile(string userName);
=======
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
>>>>>>> a804d5663436088e89f295512b47cd86722dd1e8
};

