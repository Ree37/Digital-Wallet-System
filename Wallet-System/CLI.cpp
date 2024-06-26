#include "CLI.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <conio.h>


#include "MenuItems.h"

using namespace std;

void CLI::start() {
	std::locale loc("");
	std::cout.imbue(loc);
	std::cout << std::fixed << setprecision(2);
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
					ViewRequestSettingsMenu requestSettings("Request Settings");
						AcceptRequestMenu acceptRequest("Accept");
						DeclineRequestMenu declineRequest("Decline");
			MenuItem viewTransactions("View Transactions");
				ViewUserSentTransactionsMenu viewSentTransactions("Sent Transactions");
				ViewUserRecievedTransactionsMenu viewRecievedTransactions("Recieved Transactions");
				ViewUserAllTransactionsMenu viewAllTransactions("All Transactions");
            SettingsMenu settings("Settings");
                ChangePasswordMenu changePassword("Change Password");
				Enable2FAMenu enable2FA("Enable Two-Factor Authentication");
                QrCodeMenu qrCode("QR Code");
					Confirm2FAMenu confirm2FA("Confirm Two-Factor Authentication");
    RegisterUserMenu registerUser("Register User");
		AdminProfile adminProfile("Admin Profile");
			a_AllTransactions allTransactions("All Transactions");
			a_AllUsers allUsers("All Users");
			a_SearchUser searchUser("Search for a User");
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
	adminProfile.addSubMenu(&searchUser);
	searchUser.addSubMenu(&modifyUser);
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
	enable2FA.addSubMenu(&qrCode);
	qrCode.addSubMenu(&confirm2FA);
}

void CLI::navMenu() {
	while (MenuItem::currentMenuItem.top()->update()) {}
}


void CLI::clearCli() {
	std::system("cls");
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

    getInput(input);

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

void CLI::getInput(std::string& input) {
	input.clear();
	std::getline(std::cin, input);
	std::istringstream iss(input);
	iss >> input;
}


bool CLI::getPassword(std::string& pwd) {
	pwd.clear();
	bool isVisible = false;

	while (true) {
		char ch = _getch();  

		if (ch == '\r') { 
			if (!pwd.empty()) {
				std::cout << std::endl;  
				return isVisible;
			}
		}
		else if (ch == '\b') { 
			if (!pwd.empty()) {
				std::cout << "\b \b";  
				pwd.pop_back();  
			}
		}
		else if (ch == ' ') { 
			isVisible = !isVisible;

			std::cout << std::string(pwd.length(), '\b');
			std::cout << (isVisible ? pwd : std::string(pwd.length(), '*'));  
		}
		else if (ch == 0 || ch == '\xE0')
			_getch();
		else if (ch >= 1 && ch <= 255 && std::isprint(ch)) {  
			pwd += ch; 
			std::cout << (isVisible ? ch : '*');  
		}

	}
}

void CLI::getCreditCard(std::string& cc) {
	cc.clear();
	while (true) {
		char ch = _getch();
		
	    if (ch == '\r') {
			if (!cc.empty()) {
				std::cout << std::endl;
				return;
			}
		}
		else if (ch == '\b') {
			if (!cc.empty()) {
				cc.size() % 4 == 1 && cc.size() != 1 ? std::cout << "\b \b\b" : std::cout << "\b \b";
				cc.pop_back();
			}
		}
		else if (ch == 0 || ch == '\xE0')
			_getch();
		else if (cc.size() == 16){
		}
		else if (ch >= '0' && ch <= '9' && std::isprint(ch) || cc.empty() && tolower(ch) == 'x') {
			cc += ch;
			cc.size() % 4 == 1 && cc.size() != 1 ? std::cout << ' ' << ch : std::cout << ch;
		}

	}

}