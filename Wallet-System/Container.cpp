#include "Container.h"

vector<User*> Container::users;


void addUser(User* user) {
	Container::users.push_back(user);
}
