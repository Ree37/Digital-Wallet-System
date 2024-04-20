#pragma once

#include <string>

#ifdef TOTPLib_EXPORTS
#define TOTPLib_API __declspec(dllexport)
#else
#define TOTPLib_API __declspec(dllimport)
#endif

class TOTPLib_API TOTPLib {
public:
	static std::string generateSecret();
	static uint32_t getOTP(std::string key);
};