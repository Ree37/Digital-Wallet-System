#pragma once
#include <vector>
#include "User.h"
class Container
{
public:
	static vector<User*> users;
	void addUser(User* user);
	
};

