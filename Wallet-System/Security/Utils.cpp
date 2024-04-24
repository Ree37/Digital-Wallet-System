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


