#include "Admin.h"
#include "User.h"

int Admin::getIndex(string user_name) {
	int i = 0;
	for (; i <= Admin::users.size(); i++) {
		if (Admin::users[i].username == user_name) {
			return i;
		}
		else {
			continue;
		}
	}
	return i;
}

void Admin::deleteUser(string deletedUserName) {
	int position = getIndex(deletedUserName);
	if (position != -1) {
		Admin::users.erase(Admin::users.begin() + position);
		Admin::uniqueUserName.erase(deletedUserName);
	}
}

void Admin::suspendUsers(string suspendedUserName) {
	int position = getIndex(suspendedUserName);
	Admin::users.at(position).susbended_Flag = true;
}

void Admin::adjustUserBalance(string adjustedUserName, float value) {
	int position = getIndex(adjustedUserName);
	Admin::users.at(position).balance = value;
}

//void viewUsers()
//{
//	for (int i = 0; i <= users.size(); i++) {
//		cout << "Username : " << users.at(i).name << "\t Balance : " << users.at(i).balance
//			<< endl;
//		for (int j = 0; j < users.at(i).transactions.size(); i++)
//		{
//			cout << "Transaction " << j + 1 << " :" << users.at(i).transactions.at(j);
//		}
//		cout << "************************************************************************";
//	}
//}
vector<User> Admin::viewUsers()
{
	vector<User> userVec;
	for (int i = 0; i <= Admin::users.size(); i++) {
		userVec.push_back(Admin::users.at(i));
	};
	return userVec;
}

bool Admin::isUnique(string name) { return !Admin::uniqueUserName.count(name); }
