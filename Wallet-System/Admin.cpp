#include "Admin.h"
#include "User.h"
Admin::Admin(std::string name, std::string password) {
    this->name=name;
    this->password=password;

}
int Admin::getIndex(string user_name) {
	for (int i = 0; i <= users.size(); i++) {
		if (users[i].name == user_name) {
			return i;
		}
		else {
			continue;
		}
	}
}

void Admin::deleteUser(string deletedUserName) {

	int position = getIndex(deletedUserName);
	users.erase(position);
	uniqueUserName.erase(deletedUserName);
}

void Admin::suspendUsers(string suspendedUserName) {
	int position = getIndex(suspendedUserName);
	users.at(position).susbended_Flag = true;
}

void Admin::adjustUserBalance(string adjustedUserName, int value) {
	int position = getIndex(adjustedUserName);
	users.at(position).balance = value;
}

vector<User> Admin::viewUsers()
{
	vector<User> userVec;
	for (int i = 0; i <= users.size(); i++) {
		userVec.push_back(users.at(i))
	}
	return userVec;
}
vector<User> Admin::viewTransactions()
{
    return users;
}

bool Admin::isUnique(string name) { return !uniqueUserName.count(name); }
