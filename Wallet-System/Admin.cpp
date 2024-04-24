#include "Admin.h"
#include "User.h"
#include "Container.h"
Admin Admin::adminInstance("Admin","12345678");

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
	Container::users.at(adjustedUserName)->setBalance(amount);
}

Admin::Admin(const std::string& usr, const std::string& pwd)
{
	Admin::setName(usr);
	Admin::setPassword(pwd);
}
bool Admin::isUnique(string name) {
  return !Admin::uniqueUserName.count(name);
}
void Admin::addUser(string userName, string userPassword) {
    User newUser(userName,userPassword);
    Container::addUser(newUser);
}
Admin&  Admin::getInstance() {
    return adminInstance;
}

//TODO edit
