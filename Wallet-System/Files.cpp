#include "Files.h"
#include <cassert>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

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
std::string Files::timePointToString(const std::chrono::system_clock::time_point &timePoint) {
  std::time_t t = std::chrono::system_clock::to_time_t(timePoint);
  std::tm *tm = std::localtime(&t);

  std::ostringstream oss;
  oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");

  return oss.str();
}
std::chrono::system_clock::time_point stringToTimePoint(const std::string &str) {
  std::tm tm = {};
  std::istringstream iss(str);
  iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

  std::time_t t = std::mktime(&tm);

  return std::chrono::system_clock::from_time_t(t);
}
void Files::writeUsersData(unordered_map<string, User *> &User) {
  ofstream usersFile;
  // Open the file in append mode
  usersFile.open(userData);

  if (usersFile.is_open()) {
    usersFile << "UserName"
              << ",";
    usersFile << "HashedPassword"
              << ",";
    usersFile << "Balance"
              << ",";
    usersFile << "isHas2FA"
              << ",";
    usersFile << "TOTP_Secret"
              << ",";
    usersFile << "isSuspended" << endl;

    // Input Sanitization: wrap in quotes to prevent comma insertion which
    // breaks csv
    // TODO: Sanitize Excel formula syntax to prevent csv injection

    for (auto &user : User) {
      // Note: Order is important
      usersFile << "\"" << user.second->getUsername() << "\",";
      usersFile << "\"" << user.second->getPassword() << "\",";
      usersFile << "\"" << user.second->getBalance() << "\",";
      usersFile << "\"" << user.second->getIsHas2FA() << "\",";
      usersFile << "\"" << user.second->getTotpSecret() << "\",";
      usersFile << "\"" << user.second->getSuspendedFlag() << "\"," << endl;
    }
  }

  usersFile.close();
  Utils::encryptFiles(userData);
}
void Files::writeTransactionsData(vector<Transaction *> allTransactions) {
  ofstream TransactionsFile;
  // Open the file in append mode
  TransactionsFile.open(transactionsData);

  if (TransactionsFile.is_open()) {
    TransactionsFile << "Sender"
                     << ",";
    TransactionsFile << "Recipient"
                     << ",";
    TransactionsFile << "Amount"
                     << ",";
    TransactionsFile << "IsPending"
                     << ",";
    TransactionsFile << "Date" << endl;
    // Input Sanitization: wrap in quotes to prevent comma insertion which
    // breaks csv
    // TODO: Sanitize Excel formula syntax to prevent csv injection

    for (auto &T : allTransactions) {
      // Note: Order is important
      TransactionsFile << "\"" << T->getSenderUserName() << "\",";
      TransactionsFile << "\"" << T->getRecipientUserName() << "\",";
      TransactionsFile << "\"" << T->getAmount() << "\",";
      TransactionsFile << "\"" << T->getIsPending() << "\",";
      TransactionsFile << "\"" << timePointToString(T->getDateTime()) << "\","
                       << endl;
    }
  }

  TransactionsFile.close();
  Utils::encryptFiles(userData);
}

void Files::readUsersData() {
  ifstream usersFile;
  // std::stringstream stream = Utils::decryptFiles(userData);
  usersFile.open(userData);

  // Skip header
  getLineFromData(usersFile);

  while (usersFile.peek() != EOF) {
    // Note: Order is important
    stringstream Line = getLineFromData(usersFile);
    string userName = getCellFromLine(Line);
    string storedPassword = getCellFromLine(Line);
    float storedBalance = stof(getCellFromLine(Line));
    bool setIsHas2FA = stoi(getCellFromLine(Line));
    string TOTP_Secret = getCellFromLine(Line);
    bool isSuspended = stoi(getCellFromLine(Line));

    User *user = new User(userName, storedPassword);

    user->setBalance(storedBalance);
    user->setIsHas2FA(setIsHas2FA);
    user->setTotpSecret(TOTP_Secret);
    user->setSuspendedFlag(isSuspended);

    Container::Users.insert({userName, user});
  }

  usersFile.close();
}
void Files::readTransactionsData() {
  ifstream TransactionsFile;
  // std::stringstream stream = Utils::decryptFiles(userData);
  TransactionsFile.open(transactionsData);

  // Skip header
  getLineFromData(TransactionsFile);
  while (TransactionsFile.peek() != EOF) {
    // Note: Order is important
    stringstream Line = getLineFromData(TransactionsFile);
    string sender = getCellFromLine(Line);
    string recipient = getCellFromLine(Line);
    float amount = stof(getCellFromLine(Line));
    bool isPending = stoi(getCellFromLine(Line));
    chrono::system_clock::time_point Date = stringToTimePoint(getCellFromLine(Line));

    Transaction *t = new Transaction(sender, recipient, amount);
    t->setIsPending(isPending);
    t->setDateTime(Date);
    Container::addTransaction(t);
  }
  TransactionsFile.close();
}
stringstream Files::getLineFromData(ifstream &data) {
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
    assert(false && ": Cell is not correctly formated");
  } else {
    // TODO: Make sure cell is a string (i.e. "")
    cell = cell.substr(start + 1, end - start - 1);
  }

  return cell;
}
