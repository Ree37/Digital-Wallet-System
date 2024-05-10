#include "CLI.h"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <conio.h>


#include "MenuItems.h"

using namespace std;

void CLI::start() {
	std::locale loc("");
	std::cout.imbue(loc);
	CLI::initMenu();
	CLI::navMenu();
}

MainMenu mmainMenu("Main Menu");
    LoginUserMenu loginUser("Login User");
        UserProfileMenu userProfile("User Profile");		
            SendMoneyMenu sendMoney("Send Money");
            RequestMoneyMenu requestMoney("Request Money");
            AddMoneyMenu addMoney("Add Money");
			ViewRequestsMenu viewRequests("View Requests");
				ViewToUserRequestsMenu toRequests("Requests to you");
				ViewFromUserRequestsMenu fromRequests("Requests from you");
					ViewRequestSettingsMenu requestSettings("Reqeust Settings");
						AcceptRequestMenu acceptRequest("Accept");
						DeclineRequestMenu declineRequest("Decline");
			MenuItem viewTransactions("View Transactions");
				ViewUserSentTransactionsMenu viewSentTransactions("Sent Transactions");
				ViewUserRecievedTransactionsMenu viewRecievedTransactions("Recieved Transactions");
				ViewUserAllTransactionsMenu viewAllTransactions("All Transactions");
            SettingsMenu settings("Settings");
                ChangePasswordMenu changePassword("Change Password");
                Enable2FAMenu enable2FA("Enable Two-Factor Authentication");
    RegisterUserMenu registerUser("Register User");
		AdminProfile adminProfile("Admin Profile");
			a_AllTransactions allTransactions("All Transactions");
			a_AllUsers allUsers("All Users");
				a_ModifyUserProfile modifyUser("Modify User");
					a_SetBalance setBalance("Set Balance");
					a_SuspendUser suspendUser("Suspend User");
					a_DeleteUser deleteUser("Delete User");
					



void CLI::initMenu() {
	MenuItem::currentMenuItem.push(&mmainMenu);

	mmainMenu.addSubMenu(&loginUser);
	mmainMenu.addSubMenu(&registerUser);

	loginUser.addSubMenu(&userProfile);
	loginUser.addSubMenu(&adminProfile);
	adminProfile.addSubMenu(&allTransactions);
	adminProfile.addSubMenu(&allUsers);
	adminProfile.addSubMenu(&registerUser);
	allUsers.addSubMenu(&modifyUser);
		modifyUser.addSubMenu(&viewRequests);
		modifyUser.addSubMenu(&viewTransactions);
		modifyUser.addSubMenu(&setBalance);
		modifyUser.addSubMenu(&changePassword);
		modifyUser.addSubMenu(&suspendUser);
		modifyUser.addSubMenu(&deleteUser);
	
	registerUser.addSubMenu(&userProfile);

	userProfile.addSubMenu(&sendMoney);
	userProfile.addSubMenu(&requestMoney);
	userProfile.addSubMenu(&addMoney);
	userProfile.addSubMenu(&viewRequests);

	viewRequests.addSubMenu(&toRequests);
	toRequests.addSubMenu(&requestSettings);
	viewRequests.addSubMenu(&fromRequests);

	requestSettings.addSubMenu(&acceptRequest);
	requestSettings.addSubMenu(&declineRequest);

	userProfile.addSubMenu(&viewTransactions);
	
	viewTransactions.addSubMenu(&viewSentTransactions);
	viewTransactions.addSubMenu(&viewRecievedTransactions);
	viewTransactions.addSubMenu(&viewAllTransactions);


	userProfile.addSubMenu(&settings);

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


string CLI::invalidMessage(int size, bool sorted) {

	return sorted ? "Invalid choice. Please enter 'x', 'a', 'd' or a number between 1 and " + std::to_string(size) : "Invalid choice. Please enter 'x' or a number between 1 and " + std::to_string(size);
}
void CLI::drawCli(bool isValid) {
	bool defaultMenu = (!dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem.top()) && !dynamic_cast<ViewRequestSettingsMenu*>(MenuItem::currentMenuItem.top()) && !dynamic_cast<AdminProfile*>(MenuItem::currentMenuItem.top()) && !dynamic_cast<a_ModifyUserProfile*>(MenuItem::currentMenuItem.top()) );
	
	clearCli();

	if (!isValid) {
		int size = MenuItem::currentMenuItem.top()->getSubMenus().size();
		cout << invalidMessage(size) << "\n";
	}

	MenuItem::currentMenuItem.top()->customHeader();


	MenuItem::printMenu(MenuItem::currentMenuItem.top());

	string eofTerminal = "\033[9999H";
	cout << eofTerminal;

	cout << "Enter your choice: ";
}

int CLI::getChoice(bool overwrite, int size, bool sorted) {
	
	int sizee = overwrite ? size : MenuItem::currentMenuItem.top()->getSubMenus().size();

	int choice;
	string input;

	input = getInput();

	try {
		choice = stoi(input);
	}
	catch (invalid_argument e) {
		choice = input[0];
	}

	if (choice >= 1 && choice <= sizee) {
		return choice;
	}

	if (tolower(choice) == 'x') {
		return tolower(choice);
	}

	if ((tolower(choice) == 'a' || tolower(choice) == 'd') && sorted) {
		return choice;
	}


	return false;
	
}

string CLI::getInput() {

	string input;
	std::getline(std::cin, input);
	std::istringstream iss(input);
	iss >> input;

	return input;
}


std::string CLI::getPassword() {
	std::string input;
	bool isVisible = false;

	while (true) {
		char ch = _getch();  

		if (ch == '\r') { 
			if (!input.empty()) {
				std::cout << std::endl;  
				return input;  
			}
		}
		else if (ch == '\b') { 
			if (!input.empty()) {
				std::cout << "\b \b";  
				input.pop_back();  
			}
		}
		else if (ch == ' ') { 
			isVisible = !isVisible;

			std::cout << std::string(input.length(), '\b');
			std::cout << (isVisible ? input : std::string(input.length(), '*'));  
		}
		else if (ch == 0 || ch == '\xE0')
			_getch();
		else if (ch >= 1 && ch <= 255 && std::isprint(ch)) {  
			input += ch; 
			std::cout << (isVisible ? ch : '*');  
		}

	}
}