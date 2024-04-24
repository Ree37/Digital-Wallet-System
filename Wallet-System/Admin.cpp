#include "Admin.h"
#include "User.h"
#include "Container.h"

//Admin Admin::adminInstance();

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

void Admin::editUser(string name, string newName)
{
	Container::getUser(name)->setUsername(newName);
}

void Admin::addUser(string userName, string userPassword) {
    User newUser(userName,userPassword);
    Container::addUser(&newUser);
}


//TODO edit
