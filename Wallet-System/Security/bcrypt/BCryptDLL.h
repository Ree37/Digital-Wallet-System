#pragma once

#include <string>

#ifdef BCryptDLL_EXPORTS
#define BCryptDLL_API __declspec(dllexport)
#else
#define BCryptDLL_API __declspec(dllimport)
#endif

class BCryptDLL_API BCryptDLL {
public:
    static std::string generateHash(const std::string& password, int workFactor);
    static bool validatePassword(const std::string& password, const std::string& hash);
};