#include "Admin.h"
#include "User.h"
#include "Container.h"

int Admin::getIndex(string user_name) {
	int i = 0;
	for (; i <= Container::users.size(); i++) {
		if (Container::users.at(i)->getUsername() == user_name) {
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
		Container::users.erase(Container::users.begin() + position);
		Admin::uniqueUserName.erase(deletedUserName);
	}
}

void Admin::setSuspendUsers(string suspendedUserName) {
	int position = getIndex(suspendedUserName);
	Container::users.at(position)->setSuspendedFlag();
}

void Admin::adjustUserBalance(string adjustedUserName, float value) {
	int position = getIndex(adjustedUserName);
	Container::users.at(position)->setBalance(value);
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
vector<User*> Admin::viewUsers()
{
	vector<User*> userVec;
	for (int i = 0; i <= Container::users.size(); i++) {
		userVec.push_back(Container::users.at(i));
	};
	return userVec;
}


bool Admin::isUnique(string name) { return !Admin::uniqueUserName.count(name); }
