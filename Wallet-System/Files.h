#pragma once
#include "Container.h"
#include "user.h"
#include <fstream>
#include <iostream>
#include <string>

class Files {


public:
  static void create();
  static void writeUsersDataFromContainerToFiles(unordered_map<string, User *> &User);
  //void writeTransactionsDataFromContainerToFiles(vector<Transaction *> Transactions);
  stringstream getLineFromData(ifstream &data);
  string getCellFromLine(stringstream &record);
  static void readDataFromCsvFile();
  void updateisHas2FA(User user);
  void updateTotpSecret(User user);
  void update2FA(User *user, bool isHas2FA, std::string secret);
};
