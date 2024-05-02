#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include "Utils.h"
#include "AES/AES.h"
#include <random>

const unsigned char Utils::key[32] = {
    0x6a, 0x3b, 0x5e, 0xfa, 0x1c, 0x90, 0x2d, 0x7f,
    0x82, 0x49, 0x6e, 0x57, 0x31, 0xc8, 0x04, 0x8a,
    0xda, 0x28, 0x3f, 0x76, 0xe2, 0x99, 0xbf, 0x52,
    0x1d, 0xac, 0x0b, 0x84, 0x9d, 0xfe, 0x65, 0x73
};

bool Utils::checkPasswordPolicy(std::string password) {
    bool has_upper = false, has_lower = false, has_digit = false, has_punct = false;
    for (char c : password) {
        if (isupper(c)) {
            has_upper = true;
        }
        if (islower(c)) {
            has_lower = true;
        }
        if (isdigit(c)) {
            has_digit = true;
        }
        if (ispunct(c)) {
            has_punct = true;
        }
    }

    if (!(has_upper && has_lower && has_digit && has_punct) || password.length() < 8) {
        return false;
    }

	return true;
}

int Utils::getConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void generateIV(unsigned char* iv, size_t ivSize) {
    std::random_device rd;

    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0, 255); // Unsigned char range

    for (size_t i = 0; i < ivSize; ++i) {
        iv[i] = dis(gen);
    }
}

void Utils::encryptFiles(string inputFile)
{
    ifstream inFile(inputFile, ios::binary);
    ofstream fileOut(inputFile + ".enc", ios::binary);
    string usersContent((istreambuf_iterator<char>(inFile)), (istreambuf_iterator<char>()));
    inFile.close();
    unsigned int usersContentLen = usersContent.size();
    int paddingSize = 16 - (usersContentLen % 16); // Calculate padding size
    if (paddingSize != 16) {
        usersContent.append(paddingSize, '\0'); // Pad with null bytes
    }

    usersContentLen = usersContent.size();
    
    unsigned char iv[IV_SIZE];

    generateIV(iv, IV_SIZE);

    AES aes(AESKeyLength::AES_256);
    unsigned char* usersEncryptedData = aes.EncryptCBC((unsigned char*)usersContent.c_str(), usersContentLen, key, iv);
    fileOut.write((char*)(iv), IV_SIZE);
    fileOut.write((char*)usersEncryptedData, usersContentLen);
    fileOut.close();
    delete[] usersEncryptedData;
}

std::stringstream Utils::decryptFiles(std::string inputFile) {
    std::ifstream inFile(inputFile + ".enc", std::ios::binary);
    if (!inFile.is_open()) {
        throw exception("Error: Unable to open file.");
    }

    unsigned char iv[IV_SIZE];
    inFile.read((char*)(iv), IV_SIZE);

    string encryptedContent(std::istreambuf_iterator<char>(inFile), {});
    inFile.close();

    AES aes(AESKeyLength::AES_256);
    unsigned char* decryptedData = aes.DecryptCBC((unsigned char*)encryptedContent.c_str(), encryptedContent.size(), key, iv);

    std::string decryptedString((char*)(decryptedData));

    delete[] decryptedData;

    std::stringstream stream(decryptedString);

    return stream;
}

int Utils::checkCreditCard(long long cc)
{
    // Applying Luhn Algorithm
    int digit_count = 0;
    int sum = 0;
    int amex = cc / pow(10, 13);
    int master = cc / pow(10, 14);
    int visa[2];
    visa[0] = cc / pow(10, 12);
    visa[1] = cc / pow(10, 15);

    while (cc > 0)
    {
        int digit = cc % 10;
        cc /= 10;
        digit_count++;

        if (digit_count % 2 == 0)
        {
            digit *= 2;
            if (digit > 9)
            {
                digit -= 9;
            }
        }

        sum += digit;
    }

    if (sum % 10 != 0)
        return -1;

    if (amex == 34 || amex == 37)
    {
        return 1; // AMEX
    }
    if (master == 51 || master == 52 || master == 53 || master == 54 || master == 55)
    {
        return 2; // MASTERCARD
    }
    if (visa[0] == 4 || visa[1] == 4)
    {
        return 3; // VISA
    }

    return -1;
}


string Utils::timePointToString(const chrono::system_clock::time_point& timePoint) {
    // Get the current time as a time_t value
    time_t current_time = chrono::system_clock::to_time_t(timePoint);

    // Convert to a human-readable string using strftime
    char readable_time_c[26];
    tm tm_result;
    localtime_s(&tm_result, &current_time);
    int err = strftime(readable_time_c, sizeof(readable_time_c), "%a %b %d %H:%M:%S %Y", &tm_result);

    if (err == 0) {
        throw invalid_argument("Error getting the current time.");
    }
    string readable_time_str(readable_time_c);

    string str = readable_time_str.substr(0,24);
    return str;
}

std::chrono::system_clock::time_point Utils::stringToTimePoint(const std::string& str) {
    std::tm tm = {};
    std::istringstream iss(str);

    if (!(iss >> std::get_time(&tm, "%a %b %d %H:%M:%S %Y"))) {
        throw invalid_argument("Failed to parse time");
    }

    

    std::time_t t = std::mktime(&tm);

    if (tm.tm_isdst == 1)
        t -= 60 * 60; //adjust for daylight saving
    return std::chrono::system_clock::from_time_t(t);
}