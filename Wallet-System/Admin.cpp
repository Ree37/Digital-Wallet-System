#include "Admin.h"
#include "User.h"
#include "Container.h"
#include <algorithm>
//Admin Admin::adminInstance();

Admin::Admin(string name, string pwd) : User(name,pwd) {};

void Admin::deleteUser(string deletedUserName) {
	Container::Users.erase(deletedUserName);
}

void Admin::setSuspendUsers(string suspendedUserName) {
	Container::Users.at(suspendedUserName)->setSuspendedFlag(
		!Container::Users.at(suspendedUserName)->getSuspendedFlag());
}

void Admin::adjustUserBalance(string adjustedUserName, double amount) {
	Container::Users.at(adjustedUserName)->setBalance(amount);
}

void Admin::editUser(string name, string newPwd)
{
	//Container::getUser(name)->setUsername(newName);
	Container::getUser(name)->setPassword(newPwd);
}

void Admin::addUser(string userName, string userPassword) {
	User* newUser = new User(userName, userPassword);
	Container::addUser(newUser);
}

vector<User*> Admin::viewUsers() {
	vector<User*> usrVec;
	for (auto it = Container::Users.begin(); it != Container::Users.end(); ++it) {
		usrVec.push_back(it->second);
	}

	sort(usrVec.begin(), usrVec.end(), compareByUsername);
	return usrVec;
}


vector<Transaction*>Admin::viewAllUsersTransactions() {
	vector<Transaction*> all=Container::allTransactions;
	
	sort(all.begin(), all.end(), Transaction::oldestDate);
	return all ;
}

//TODO edit
