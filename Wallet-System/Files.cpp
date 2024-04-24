#include "Files.h"
#include "Container.h"
#include "Security/Utils.h"
#include "User.h"
#include <sstream>
string transactionsData = "Transactions.csv";
string userData = "UsersData.csv";
using namespace std;
void Files::create() {
  // check if file already exists else create it
  ofstream usersFile, Transactions;
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
  if (Transactions.is_open()) {
    Transactions << "Sender"
                 << ",";
    Transactions << "recipient"
                 << ",";
    Transactions << "amount" << endl;
  }
  usersFile.close();
  Transactions.close();
}
void Files::writeUsersDataFromContainerToFiles(
    unordered_map<string, User *> &User) {
  ofstream usersFile;
  usersFile.open(userData, ios::app); // Open the file in append mode
  if (usersFile.is_open()) {
    // Input Sanitization: wrap in quotes to prevent comma insertion which
    // breaks csv
    // TODO: Sanitize Excel formula syntax to prevent csv injection

    for (auto &user : User) {
      usersFile << "\"" << user.second->getUsername() << "\",";
      usersFile << "\"" << user.second->getPassword() << "\",";
      usersFile << "\"" << user.second->getBalance() << "\"";
      usersFile << "\"" << user.second->getIsHas2FA() << "\"";
      usersFile << "\"" << user.second->getTotpSecret() << "\"";
      usersFile << "\"" << user.second->getSuspendedFlag() << "\"" << endl;
    }
  }
  usersFile.close();
}
// void Files::writeTransactionsDataFromContainerToFiles(
//     vector<Transaction> Transactions) {
//   ofstream Transactionsfile;
//   Transactionsfile.open(transactionsData,
//                         ios::app); // Open the file in append mode
//   if (Transactionsfile.is_open()) {
//     for (Transaction trans : Transactions) {
//     }
//   }
// }
stringstream getLineFromData(ifstream &data) {
  string records;
  getline(data, records);
  stringstream record(records);
  return record;
}
string getCellFromLine(stringstream &record) {
  string cell;
  getline(record, cell, ',');
  size_t start = cell.find_first_not_of(" \t");
  size_t end = cell.find_last_not_of(" \t");
  if (start == string::npos || end == string::npos) {
    cell = ""; // Empty string if no non-whitespace characters found
  } else {
    cell = cell.substr(start, end - start + 1);
  }
  return cell;
}
void readDataFromCsvFile() {
  ifstream usersFile;
  usersFile.open(userData);
  while (usersFile.peek() != EOF) {
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

void Files::updateisHas2FA(User user) {
  ifstream usersfile;
  usersfile.open(userData);
  string records;
  string tempFileName = "temp.csv"; // Temporary file to store modified content
  ofstream tempfile(tempFileName);

  while (std::getline(usersfile, records)) {
    stringstream record(records);
    string userName;
    string cell;
    string result = "";

    if (getline(record, userName, ',')) {
      if (userName == user.getUsername()) {
        result += '\"' + userName + '\"' + ',';
        getline(record, cell, ',');
        result += '\"' + cell + '\"' + ',';
        getline(record, cell, ',');
        result += '\"' + cell + '\"' + ',';
        getline(record, cell, ',');
        result +=
            '\"' + std::to_string(user.getIsHas2FA() ? 1 : 0) + '\"' + ',';
        getline(record, cell, ',');
        result += '\"' + cell + '\"';
        tempfile << result << endl; // Write modified line to temporary file
      } else {
        tempfile << records << endl; // Write unchanged line to temporary file
      }
    }
  }

  usersfile.close();
  tempfile.close();

  // Replace the original file with the temporary file
  remove(userData.c_str()); // Remove the original file
  rename(tempFileName.c_str(),
         userData.c_str()); // Rename temporary file to original file
}

void Files::updateTotpSecret(User user) {
  ifstream usersfile;
  usersfile.open(userData);
  string records;
  string tempFileName = "temp.csv"; // Temporary file to store modified content
  ofstream tempfile(tempFileName);

  while (std::getline(usersfile, records)) {
    stringstream record(records);
    string userName;
    string cell;
    string result = "";

    if (getline(record, userName, ',')) {
      if (userName == user.getUsername()) {
        result += '\"' + userName + '\"' + ',';
        getline(record, cell, ',');
        result += '\"' + cell + '\"' + ',';
        getline(record, cell, ',');
        result += '\"' + cell + '\"' + ',';
        getline(record, cell, ',');
        result += '\"' + cell + '\"' + ',';
        getline(record, cell, ',');
        result += '\"' + user.getTotpSecret() + '\"';
        tempfile << result << endl; // Write modified line to temporary file
      } else {
        tempfile << records << endl; // Write unchanged line to temporary file
      }
    }
  }

  usersfile.close();
  tempfile.close();

  // Replace the original file with the temporary file
  remove(userData.c_str()); // Remove the original file
  rename(tempFileName.c_str(),
         userData.c_str()); // Rename temporary file to original file
}

void Files::update2FA(User *user, bool isHas2FA, std::string secret) {
  user->setIsHas2FA(true);
  user->setTotpSecret(secret);
  updateisHas2FA(*user);
  updateTotpSecret(*user);
}
