#pragma once
#include <string>
using namespace std;


class Utils
{
public:
	static bool checkPasswordPolicy(string password);
	static int getConsoleWidth();
	static void encryptFiles(string inputFile);
	static std::stringstream decryptFiles(string inputFile);
private:
	static const size_t IV_SIZE = 16;
	static const unsigned char key[32];
};

