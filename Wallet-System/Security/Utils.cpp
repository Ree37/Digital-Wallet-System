#include <string>
#include <iostream>
#include <Windows.h>
#include "Utils.h"

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