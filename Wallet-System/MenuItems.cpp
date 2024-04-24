#include "MenuItems.h"

#include <windows.h>
#include <synchapi.h>
#include <conio.h>
#include <iostream>

#include "Security/Utils.h"
#include "Security/bcrypt/BCryptLib.h"
#include "Security/qrcode/QrcodeLib.h"
#include "Security/totp/TOTPLib.h"
#include "User.h"
#include "Container.h"

using namespace std;

User* MenuItem::user;
stack <MenuItem*> MenuItem::currentMenuItem;

MenuItem::MenuItem(std::string name) : name(name) {}

void MenuItem::printMenu(MenuItem* menuItem) {
	for (size_t i = 0; i < menuItem->getSubMenus().size(); i++) {
		cout << i + 1 << "] " << menuItem->getSubMenus().at(i)->name << "\n";
	}

	if (dynamic_cast<MainMenu*>(MenuItem::currentMenuItem.top())) {
		cout << "x] Exit" << "\n";
	}
	else if (dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem.top())) {
		cout << "x] Log Out" << "\n";
	}
	else if (menuItem->hasBack()) {
		cout << "x] Back" << "\n";
	}
}

std::string MenuItem::getName() {
	return this->name;
}

void MenuItem::setName(std::string name) {
	this->name = name;
}

std::vector<MenuItem*> MenuItem::getSubMenus() {
	return this->subMenus;
}

bool MenuItem::hasSubMenus() {
	return !subMenus.empty();
}

bool MenuItem::hasBack() {
	return true;
}

void MenuItem::addSubMenu(MenuItem* subMenu) {
	subMenus.push_back(subMenu);
}

bool MenuItem::update() {
	bool isValid = true;
	int choice;
	do {
		CLI::drawCli(isValid);
		choice = CLI::getInput();

		if (choice == 0) {
			isValid = false;
		}
		else {
			isValid = true;
		}
	} while (!isValid);

	if (choice == 'x') {
		return back();
	}
	else {
		currentMenuItem.push(currentMenuItem.top()->getSubMenus()[choice - 1]);
	}
	return true;
}

bool MenuItem::back() {
	currentMenuItem.pop();
	return true;
}

bool MenuItem::exitCommand(string s) {
	if (s.length() == 1 && tolower(s[0]) == 'x')
	{
		back();
		return true;
	}

	return false;
}

MainMenu::MainMenu(string name) : MenuItem(name) {};

bool MainMenu::back() {

	CLI::clearCli();

	cout << "Exiting...\n";

	return false;
};

LoginUserMenu::LoginUserMenu(string name) : MenuItem(name) {};

bool LoginUserMenu::update() {

	CLI::clearCli();
	cout << "Input 'x' to leave login menu\n\n";
	while (true) {
		string username, password;

		cout << "Enter Username: ";
		cin >> username;

		if (exitCommand(username))
			return true;

		cout << "Enter Password: ";
		cin >> password;

		if (exitCommand(password))
			return true;

		User* userData = Container::getUser(username);
		try {
			if (!userData) {
				throw exception();
			}
			if (!BCryptLib::validatePassword(password, userData->getPassword())) {
				throw exception();
			}
		}
		catch (exception e) {
			CLI::clearCli();
			cout << "Input 'x' to leave login menu\n\n";
			cout << "Wrong Username or Password\n\n";
			continue;
		}

		if (userData->getIsHas2FA())
		{
			string i_otp;
			string secret = userData->getTotpSecret();
			QrcodeLib* qrcode = new QrcodeLib(userData->getUsername(), secret);
			cout << "Enter 2FA Key: ";
			cin >> i_otp;

			if (exitCommand(i_otp))
				return true;

			unsigned int otp = TOTPLib::getOTP(secret);

			if (stoi(i_otp) != otp)
			{
				CLI::clearCli();
				cout << "Invalid 2FA Key\n\n";
				continue;
			}
		}

		MenuItem::user = userData;
		currentMenuItem.push(currentMenuItem.top()->getSubMenus()[0]);

		break;
	}

};

RegisterUserMenu::RegisterUserMenu(string name) : MenuItem(name) {};

bool RegisterUserMenu::update() {
	CLI::clearCli();

	string username, password;
	cout << "Input 'x' to leave register menu\n";

	cout << "Enter Username: ";
	cin >> username;

	if (exitCommand(username))
		return true;

	while (true) {
		try {
			cout << "Enter Password: ";
			cin >> password;

			if (exitCommand(password))
				return true;

			if (!Utils::checkPasswordPolicy(password))
			{
				throw std::invalid_argument("Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long.");
			}
			string hash = BCryptLib::generateHash(password, 12);

			MenuItem::user = new User(username, hash);

			Container::addUser(MenuItem::user);

			currentMenuItem.push(currentMenuItem.top()->getSubMenus()[0]);

			break;
		}
		catch (exception e) {
			CLI::clearCli();
			cout << "Input 'x' to leave register menu\n\n";

			cout << e.what() << '\n';
			cout << "Enter Username: " << username << "\n";
		}
	}
};


UserProfileMenu::UserProfileMenu(string name) : MenuItem(name) {};

bool UserProfileMenu::update() {
	CLI::clearCli();

	cout << "\nCurrent User: " << MenuItem::user->getUsername() << "\n\n";
	cout << "Available Balance: " << MenuItem::user->getBalance() << "\n\n";

	MenuItem::update();

	return true;
};

bool UserProfileMenu::back() {
	currentMenuItem.pop();
	currentMenuItem.pop();

	return true;
};

SettingsMenu::SettingsMenu(string name) : MenuItem(name) {};

bool SettingsMenu::update() {
	if (user->getIsHas2FA())
	{
		currentMenuItem.top()->getSubMenus().at(1)->name = "Disable Two-Factor Authentication";
	}
	else
	{
		currentMenuItem.top()->getSubMenus().at(1)->name = "Enable Two-Factor Authentication";
	}

	MenuItem::update();

	return true;
};

ChangePasswordMenu::ChangePasswordMenu(string name) : MenuItem(name) {};

//update yet to be implemented
bool ChangePasswordMenu::update() {

	MenuItem::update();

	return true;
};

//override back if needed

Enable2FAMenu::Enable2FAMenu(string name) : MenuItem(name) {};

bool Enable2FAMenu::update() {
	CLI::clearCli();

	if (user->getIsHas2FA())
	{
		std::string i_otp;

		string secret = user->getTotpSecret();
		QrcodeLib* qrcode = new QrcodeLib(user->getUsername(), secret);
		cout << "Input 'x' to leave 2FA menu\n";
		cout << "Enter 2FA Key: ";
		cin >> i_otp;

		if (exitCommand(i_otp))
			return true;

		unsigned int otp = TOTPLib::getOTP(secret);

		if (stoi(i_otp) != otp)
		{
			CLI::clearCli();
			cout << "Invalid 2FA Key\n\n";
			return true;
		}

		CLI::clearCli();
		cout << "Press any key to go back\n\n";

		user->setIsHas2FA(false);
		user->setTotpSecret("");

		cout << "2FA has been disabled!\n";
		while (!_kbhit()) {
		}

		_getch();
		back();

		return true;
	}

	string password;
	cout << "Input 'x' to leave 2FA menu\n";
	cout << "Enter Current Password to enable 2FA: ";
	cin >> password;

	if (exitCommand(password))
		return true;

	if (!BCryptLib::validatePassword(password, user->getPassword())) {
		return true;
	}

	CLI::clearCli();

	std::string secret = TOTPLib::generateSecret();
	QrcodeLib* qrcode = new QrcodeLib(user->getUsername(), secret);

	user->setIsHas2FA(true);
	user->setTotpSecret(secret);

	qrcode->printQRCodeASCII();
	cout << "Press any key to go back\n\n";

	int width = Utils::getConsoleWidth();
	int new_width;

	while (!_kbhit())
	{
		new_width = Utils::getConsoleWidth();

		if (new_width != width)
		{
			CLI::clearCli();
			qrcode->printQRCodeASCII();
			cout << "\nPress any key to go back\n\n";
			width = new_width;
		}

		Sleep(300);
	}

	_getch();
	back();

	return true;
}

