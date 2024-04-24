#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "user.h"

class Files {
public:
	static void create();
	static void writeUsersDataFromContainerToFiles(unordered_map<string, User*>& User);
	static void readDataFromCsvFile();

private:
	static stringstream getLineFromData(ifstream& data);
	static string getCellFromLine(stringstream& record);
};

