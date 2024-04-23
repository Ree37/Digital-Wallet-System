#pragma once

#include <string>

#ifdef QrcodeLib_EXPORTS
#define QrcodeLib_API __declspec(dllexport)
#else
#define QrcodeLib_API __declspec(dllimport)
#endif

class QrcodeLib_API QrcodeLib {
public:
	QrcodeLib(std::string username, std::string secret);
	std::string generateQrcode(std::string url);
	std::string createTotpUrl(std::string username, std::string secret);
	void printQRCodeASCII();
};