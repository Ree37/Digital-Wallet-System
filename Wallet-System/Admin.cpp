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
string Admin::removeSpecialCharsAndNumbers(string& str) {
	string result;
	for (char c : str) {
		if (isalpha(c) || isspace(c)) {
			result += c;
		}
	}
	return result;
}
string Admin::preprocessing(string name) {
	transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return tolower(c); });
	return removeSpecialCharsAndNumbers(name);
}
vector<User*> Admin::search(string name) {

	string preprocessedName = preprocessing(name);
	int maxNumberOfDifference = 5;
	vector<User*> expected;
	unordered_map<char, int> countChars1;
	for (char c : preprocessedName) {
		countChars1[c]++;
	}
	unordered_map<string, User*>::iterator it2;
	for(it2=Container::Users.begin();it2!= Container::Users.end();it2++){
		string preprocessedName2 = preprocessing(it2->first);
		unordered_map<char, int> countChars2;
		for (char c : preprocessedName2) {
			countChars2[c]++;
		}

		int numberOfDifference = 0;
		for (auto it = countChars2.begin(); it != countChars2.end(); it++) {
			if (countChars1.count(it->first)) {
				numberOfDifference += abs(countChars1[it->first] - it->second);
			}
			else {
				numberOfDifference += it->second;
			}
		}

		numberOfDifference += abs(static_cast<int>(preprocessedName2.size() - preprocessedName.size())); // Add the size difference
		if (numberOfDifference <= maxNumberOfDifference) {
			expected.push_back(it2->second);
		}
	}

	return expected;
}
//TODO edit
