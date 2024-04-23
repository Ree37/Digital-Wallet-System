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
public:
	void writeUsersDataFromContainerToFiles(vector <User*> users);
	void writeTransactionsDataFromContainerToFiles(vector<Transaction>Transactions);
	stringstream getLineFromData(ifstream &data);
	string getCellFromLine(stringstream &record);
	User* readDataFromCsvFile(string userName);
};

