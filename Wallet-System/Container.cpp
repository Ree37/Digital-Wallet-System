#include "Container.h"

unordered_map<string, User*> Container::Users;
vector<Transaction *> Container:: allTransactions;

unordered_map<string, vector<Transaction *>> Container::userKeyTransactions;
void Container::addUser(User* user) {
	Users.insert({ user->getUsername(), user });
}

User* Container::getUser(string userName) {
	if (Users.count(userName)) {
		return Users[userName];
	}
	else {
		return nullptr;
	}
}
void Container ::addTransaction(Transaction *t) { 
	allTransactions.push_back(t);
	userKeyTransactions[t->getSenderUserName()].push_back(t);
    userKeyTransactions[t->getRecipientUserName()].push_back(t);
}
vector<Transaction*> Container::getSentTransaction(string senderUserName) {
  vector<Transaction*> allUserTransaction = userKeyTransactions[senderUserName];
  vector<Transaction *> sentUserTransactions ;
	for (auto t : allUserTransaction ) {
    if (t->getSenderUserName()== senderUserName) {
            sentUserTransactions.push_back(t);
    }
  }
  return sentUserTransactions;
}
vector<Transaction *> Container::getRecipientTransaction(string RecipientUserName) {
  vector<Transaction *> allUserTransaction =  userKeyTransactions[RecipientUserName];
  vector<Transaction *> RecipientUserTransactions;
  for (auto t : allUserTransaction) {
    if (t->getSenderUserName() == RecipientUserName) {
      RecipientUserTransactions.push_back(t);
    }
  }
  return RecipientUserTransactions;
}
vector<Transaction *> Container::getAllUserTransaction(string UserName) {
  return userKeyTransactions[UserName];
}



