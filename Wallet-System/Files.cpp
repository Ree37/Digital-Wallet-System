#include "Files.h"
#include <cassert>
#include <sstream>
#include <chrono>
#include "Container.h"
#include "Security/Utils.h"
#include "User.h"

using namespace std;

string transactionsData = "Transactions.csv";
string userData = "UsersData.csv";

void Files::create() {
  // check if file already exists else create it
  ofstream usersFile;
  ofstream Transactions;

  Transactions.open(transactionsData);
  usersFile.open(userData);

  if (usersFile.is_open()) {
    usersFile << "UserName"
              << ",";
    usersFile << "Password"
              << ",";
    usersFile << "Balance"
              << ",";
    usersFile << "isHas2FA"
              << ",";
    usersFile << "TOTP_Secret"
              << ",";
    usersFile << "isSuspended" << endl;
  }
}


void Files::writeUsersData(unordered_map<string, User *> &User) {
  stringstream usersData;
  usersData << std::fixed << std::setprecision(2);

    usersData << "UserName"
              << ",";
    usersData << "HashedPassword"
              << ",";
    usersData << "Balance"
              << ",";
    usersData << "isHas2FA"
              << ",";
    usersData << "TOTP_Secret"
              << ",";
    usersData << "isSuspended" << endl;

    // Input Sanitization: wrap in quotes to prevent comma insertion which
    // breaks csv
    // TODO: Sanitize Excel formula syntax to prevent csv injection

    for (auto &user : User) {
      // Note: Order is important
      usersData << "\"" << user.second->getUsername() << "\",";
      usersData << "\"" << user.second->getPassword() << "\",";
      usersData << "\"" << user.second->getBalance() << "\",";
      usersData << "\"" << user.second->getIsHas2FA() << "\",";
      usersData << "\"" << user.second->getTotpSecret() << "\",";
      usersData << "\"" << user.second->getSuspendedFlag() << "\"" << endl;
      delete user.second;
    }

  Utils::encryptFiles(userData, usersData);
}
void Files::writeTransactionsData(vector<Transaction *> allTransactions) {
   stringstream TransactionsData;
   TransactionsData << std::fixed << std::setprecision(2);

    TransactionsData << "Sender"
                     << ",";
    TransactionsData << "Recipient"
                     << ",";
    TransactionsData << "Amount"
                     << ",";
    TransactionsData << "IsPending"
                     << ",";
    TransactionsData << "Date" << endl;
    // Input Sanitization: wrap in quotes to prevent comma insertion which
    // breaks csv
    // TODO: Sanitize Excel formula syntax to prevent csv injection

    for (auto &T : allTransactions) {
      // Note: Order is important
      TransactionsData << "\"" << T->getSenderUserName() << "\",";
      TransactionsData << "\"" << T->getRecipientUserName() << "\",";
      TransactionsData << "\"" << T->getAmount() << "\",";
      TransactionsData << "\"" << T->getIsPending() << "\",";
      TransactionsData << "\"" << Utils::timePointToString(T->getDateTime()) << "\"" << endl;
      delete T;
    }

  Utils::encryptFiles(transactionsData, TransactionsData);
}

void Files::readUsersData() {
  std::stringstream stream;
  try
  {
      stream = Utils::decryptFiles(userData);
  }
  catch(exception e)
  {
      return;
  }

  // Skip header
  getLineFromData(stream);

  while (stream.peek() != EOF) {
    // Note: Order is important
      try {
          stringstream Line = getLineFromData(stream);
          string userName = getCellFromLine(Line);
          string storedPassword = getCellFromLine(Line);
          double storedBalance = stod(getCellFromLine(Line));
          bool setIsHas2FA = stoi(getCellFromLine(Line));
          string TOTP_Secret = getCellFromLine(Line);
          bool isSuspended = stoi(getCellFromLine(Line));

          User* user = new User(userName, storedPassword);

          user->setBalance(storedBalance);
          user->setIsHas2FA(setIsHas2FA);
          user->setTotpSecret(TOTP_Secret);
          user->setSuspendedFlag(isSuspended);

          Container::Users.insert({ userName, user });
      }
      catch (const exception& e) {
      }
  }

  
}
void Files::readTransactionsData() {
  ifstream TransactionsFile;
  std::stringstream stream;
  try
  {
      stream = Utils::decryptFiles(transactionsData);
  }
  catch (exception e)
  {
      return;
  }

  // Skip header
  getLineFromData(stream);
  while (stream.peek() != EOF) {
    // Note: Order is important
    stringstream Line = getLineFromData(stream);
    string sender = getCellFromLine(Line);
    string recipient = getCellFromLine(Line);
    double amount = stod(getCellFromLine(Line));
    int isPending = stoi(getCellFromLine(Line));
    chrono::system_clock::time_point Date = Utils::stringToTimePoint(getCellFromLine(Line));

    Transaction *t = new Transaction(sender, recipient, amount);
    t->setIsPending(isPending);
    t->setDateTime(Date);
    Container::addTransaction(t);
  }
}
stringstream Files::getLineFromData(stringstream&data) {
  string records;
  getline(data, records);
  stringstream record(records);
  return record;
}

string Files::getCellFromLine(stringstream &record) {
  string cell;
  getline(record, cell, ',');

  size_t start = cell.find_first_of('\"');
  size_t end = cell.find_last_of('\"');

  if (start == string::npos || end == string::npos || start == end) {
    // Empty string if no non-whitespace characters found
    cell = "";
    throw invalid_argument("Cell is not correctly formated");
  } else {
    // TODO: Make sure cell is a string (i.e. "")
    cell = cell.substr(start + 1, end - start - 1);
  }

  return cell;
}
