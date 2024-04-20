#pragma once

#include <string>

#ifdef BCryptLib_EXPORTS
#define BCryptLib_API __declspec(dllexport)
#else
#define BCryptLib_API __declspec(dllimport)
#endif

class BCryptLib_API BCryptLib {
public:
    static std::string generateHash(const std::string& password, int workFactor);
    static bool validatePassword(const std::string& password, const std::string& hash);
};