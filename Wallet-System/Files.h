#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include "user.h"

class Files {
public:
	static void create();
	static void writeUsersData(unordered_map<string, User*>& User);
    static void writeTransactionsData( vector<Transaction *> allTransactions);
	static void readUsersData();
    static void readTransactionsData();

private:
	static stringstream getLineFromData(stringstream& data);
	static string getCellFromLine(stringstream& record);
    
};

