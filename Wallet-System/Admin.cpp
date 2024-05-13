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

std::string removeAdjacentDuplicates(const std::string& str) {
	if (str.empty()) return str; // Return empty string if input is empty

	std::string result;
	result.push_back(str[0]); // Add the first character to the result
	for (size_t i = 1; i < str.size(); ++i) {
		// If the current character is different from the last character added to the result, add it
		if (str[i] != result.back()) {
			result.push_back(str[i]);
		}
	}
	return result;
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
	name = removeAdjacentDuplicates(name);
	return removeSpecialCharsAndNumbers(name);
}
/*
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
*/


vector<User*> Admin::search(string name)
{
	string str1  = preprocessing(name);
	vector<User*> v;

	int m = str1.length();

	for (auto u : Container::Users)
	{
		string str2 = preprocessing(u.second->getUsername());
		int n = str2.length();

		vector<vector<int> > dp(m + 1, vector<int>(n + 1, 0));

		for (int i = 0; i <= m; i++) {
			dp[i][0] = i;
		}

		for (int j = 0; j <= n; j++) {
			dp[0][j] = j;
		}

		for (int i = 1; i <= m; i++) {
			for (int j = 1; j <= n; j++) {
				if (str1[i - 1] == str2[j - 1]) {
					dp[i][j] = dp[i - 1][j - 1];
				}
				else {
					dp[i][j] = 1
						+ min(

							// Insert
							dp[i][j - 1],
							min(

								// Remove
								dp[i - 1][j],

								// Replace
								dp[i - 1][j - 1]));
				}
			}
		}

		if (dp[m][n] <= 2) {
			v.push_back(u.second);
		}
	}
	return v;
}
