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
	static stringstream getLineFromData(ifstream& data);
	static string getCellFromLine(stringstream& record);
    static string timePointToString(const std::chrono::system_clock::time_point &timePoint);
    static chrono::system_clock::time_point stringToTimePoint(const std::string &str);
};

