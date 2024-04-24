#include "Container.h"

unordered_map<string, User*> Container::Users;

void Container::addUser(User* user) {
	Users.insert({ user->getUsername(), user });
}

User* Container::getUser(string userName) {
	if (Users.count(userName)) {
		return Users[userName];
	}
	else {
		return nullptr;
	}
}

