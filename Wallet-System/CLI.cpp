#include "CLI.h"

#include <iostream>
#include <cstdlib>

#include "MenuItems.h"

using namespace std;

void CLI::start() {
	CLI::initMenu();
	CLI::navMenu();
}

MainMenu mmainMenu("Main Menu");
    LoginUserMenu loginUser("Login User");
        UserProfileMenu userProfile("User Profile");
            SendMoneyMenu sendMoney("Send Money");
            MenuItem requestMoney("Request Money");
            AddMoneyMenu addMoney("Add Money");
            MenuItem viewRequests("View Requests");
            MenuItem viewTransactions("View Transactions");
            SettingsMenu settings("Settings");
				ChangeUserNameMenu changeUserName("Change Username");
                ChangePasswordMenu changePassword("Change Password");
                Enable2FAMenu enable2FA("Enable Two-Factor Authentication");
    RegisterUserMenu registerUser("Register User");
    MenuItem loginAdmin("Login Admin");

void CLI::initMenu() {
	MenuItem::currentMenuItem.push(&mmainMenu);

	mmainMenu.addSubMenu(&loginUser);
	mmainMenu.addSubMenu(&registerUser);
	mmainMenu.addSubMenu(&loginAdmin);

	loginUser.addSubMenu(&userProfile);
	registerUser.addSubMenu(&userProfile);

	userProfile.addSubMenu(&sendMoney);
	userProfile.addSubMenu(&requestMoney);
	userProfile.addSubMenu(&addMoney);
	userProfile.addSubMenu(&viewRequests);
	userProfile.addSubMenu(&viewTransactions);
	userProfile.addSubMenu(&settings);

	settings.addSubMenu(&changeUserName);
	settings.addSubMenu(&changePassword);
	settings.addSubMenu(&enable2FA);
}

void CLI::navMenu() {
	while (MenuItem::currentMenuItem.top()->update()) {}
}


void CLI::clearCli() {
	try {
#ifdef _WIN32
		std::system("cls");
#else
		// Assume Unix-like system
		std::cout << "\033[H\033[2J";
		std::cout.flush();
#endif
	}
	catch (const std::exception& e) {
		std::cout << "An error occurred while clearing the CLI: " << e.what() << std::endl;
	}
}

void CLI::drawCli(bool isValid) {
	if (!dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem.top())) {
		clearCli();
	}

	if (!isValid) {
		cout << "\nInvalid choice. Please enter 'x' or a number between 1 and " << MenuItem::currentMenuItem.top()->getSubMenus().size();
	}

	if (!dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem.top())) {
		cout << "\nCurrent Menu: " + MenuItem::currentMenuItem.top()->getName() << "\n\n";
	}
	MenuItem::printMenu(MenuItem::currentMenuItem.top());

	string eofTerminal = "\033[9999H";
	cout << eofTerminal;

	cout << "Enter your choice: ";
}

int CLI::getInput() {
	int choice;
	string input;

	cin >> input;

	try {
		choice = stoi(input);
	}
	catch (invalid_argument e) {
		choice = input[0];
	}

	if ((choice < 1 || choice > MenuItem::currentMenuItem.top()->getSubMenus().size()) && choice != 'x') {
		return 0;
	}
	else {
		return choice;
	}
}

