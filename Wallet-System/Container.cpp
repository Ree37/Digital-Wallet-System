#include "Container.h"

unordered_map<string, User*> Container::Users;
vector<Transaction *> Container:: allTransactions;

User* Container::admin = new Admin("admin", "$2a$12$RQ8XZhi9Tod7M2FvDgzBeeXfyKfHovi.aJDXCpstZUlY3RoVYYwFO");

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

void Container::checkUniqueUser(string username) {

	if (Users.count(username)) {
		throw invalid_argument("Username already taken!");
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
    if (t->getRecipientUserName() == RecipientUserName) {
      RecipientUserTransactions.push_back(t);
    }
  }
  return RecipientUserTransactions;
}

vector<Transaction *> Container::getAllUserTransaction(string UserName) {
  return userKeyTransactions[UserName];
}



