#include "Admin.h"
#include "User.h"
#include "Container.h"

//Admin Admin::adminInstance();

Admin* globalAdmin = new Admin();

void Admin::deleteUser(string deletedUserName) {
	Container::Users.erase(deletedUserName);
}

void Admin::setName(string name)
{
	this->name = name;
}

void Admin::setPassword(string password)
{
	this->password = password;
}

string Admin::getName()
{
	return name;
}

string Admin::getPassword()
{
	return password;
}
//vector<User*> Admin::viewUsers()
//{
//unordered_map<string,User>::
//    for (int i = 0; i <= Container::users.size(); i++) {
//        userVec.push_back(Container::users.at(i));
//    };
//    return userVec;
//}
void Admin::setSuspendUsers(string suspendedUserName) {
	Container::Users.at(suspendedUserName)->setSuspendedFlag(
		!Container::Users.at(suspendedUserName)->getSuspendedFlag());
}

void Admin::adjustUserBalance(string adjustedUserName, float amount) {
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

vector<User*> Admin::viewUsers()
{
	vector<User*> usrVec;
	unordered_map<string, User*>::iterator it;
	for (it = Container::Users.begin(); it != Container::Users.end(); it++)
	{
		usrVec.push_back(it->second);
	}
	return usrVec;
}

vector<Transaction*>Admin::viewAllUsersTransactions() {
	unordered_map<string, vector<Transaction*>>::iterator it;

	it = Container::userKeyTransactions.begin();
	vector<Transaction*> returnedTransaction;
	vector<Transaction*> specificUserTransacions;
	while (it != Container::userKeyTransactions.end()) {
		specificUserTransacions = Container::getAllUserTransaction(it->first);
		for (int i = 0; i < specificUserTransacions.size(); i++) {
			returnedTransaction.push_back(specificUserTransacions[i]);
		}

		it++;
	}
	return returnedTransaction;
}
vector<Transaction*> Admin::viewSpecicTranactioBySenderName(string senderUserName) {
	return Container::getSentTransaction(senderUserName);
}
vector<Transaction*> Admin::viewSpecicTranactioByRecipientName(string recipientName) {

	return Container::getRecipientTransaction(recipientName);
}
//TODO edit
