#pragma once
#include <iostream>
#include <string>
#include"user.h"
#include <fstream>
using namespace std;
class Files
{
public:
	void create();
	void writeData(User inputUser);
	User readData(string userName);

};

