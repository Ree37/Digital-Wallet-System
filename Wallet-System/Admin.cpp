//#include "Admin.h"
//#include "User.h"
//
//int getIndex(string user_name) {
//	for (int i = 0; i <= users.size(); i++) {
//		if (users[i].name == user_name) {
//			return i;
//		}
//		else {
//			continue;
//		}
//	}
//}
//
//void deleteUser(string deletedUserName) {
//
//	int position = getIndex(deletedUserName);
//	users.erase(position);
//	uniqueUserName.erase(deletedUserName);
//}
//
//void suspendUsers(string suspendedUserName) {
//	int position = getIndex(suspendedUserName);
//	users.at(position).susbended_Flag = true;
//}
//
//void adjustUserBalance(string adjustedUserName, int value) {
//	int position = getIndex(adjustedUserName);
//	users.at(position).balance = value;
//}
//
////void viewUsers()
////{
////	for (int i = 0; i <= users.size(); i++) {
////		cout << "Username : " << users.at(i).name << "\t Balance : " << users.at(i).balance
////			<< endl;
////		for (int j = 0; j < users.at(i).transactions.size(); i++)
////		{
////			cout << "Transaction " << j + 1 << " :" << users.at(i).transactions.at(j);
////		}
////		cout << "************************************************************************";
////	}
////}
//vector<User> viewUsers()
//{
//	vector<User> userVec;
//	for (int i = 0; i <= users.size(); i++) {
//		userVec.push_back(users.at(i));
//	}
//	return userVec;
//}
//
//bool isUnique(string name) { return !uniqueUserName.count(name); }
