#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
using namespace std;

class Utils
{
public:
	static bool checkPasswordPolicy(string password);
	static int getConsoleWidth();
	static void encryptFiles(string inputFile, const stringstream& data);
	static std::stringstream decryptFiles(string inputFile);
	static int checkCreditCard(long long cc);
	static string timePointToString(const std::chrono::system_clock::time_point& timePoint);
	static chrono::system_clock::time_point stringToTimePoint(const std::string& str);
	static void generateIV(unsigned char* iv);
private:
	static const size_t IV_SIZE = 16;
	static const unsigned char key[32];
};

