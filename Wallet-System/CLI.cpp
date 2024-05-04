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
            RequestMoneyMenu requestMoney("Request Money");
            AddMoneyMenu addMoney("Add Money");
			MenuItem viewRequests("View Requests");
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
			AllTransactions allTransactions("All Transactions");
			AllUsers allUsers("All Users");



void CLI::initMenu() {
	MenuItem::currentMenuItem.push(&mmainMenu);

	mmainMenu.addSubMenu(&loginUser);
	mmainMenu.addSubMenu(&registerUser);

	loginUser.addSubMenu(&userProfile);
	loginUser.addSubMenu(&adminProfile);
		adminProfile.addSubMenu(&allTransactions);
		adminProfile.addSubMenu(&allUsers);
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

	return sorted ? "Invalid choice. Please enter 'x', 'r', 'o' or a number between 1 and " + std::to_string(size) : "Invalid choice. Please enter 'x' or a number between 1 and " + std::to_string(size);
}
void CLI::drawInvalid() {
	int size = MenuItem::currentMenuItem.top()->getSubMenus().size();

	if (dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem.top())) {
		clearCli();
		cout << invalidMessage(size) << "\n";
		cout << "\nCurrent User: " << MenuItem::user->getUsername() << "\n\n";
		cout << "Available Balance: " << MenuItem::user->getBalance() << "\n\n";
	}
	else if(dynamic_cast<ViewRequestSettingsMenu*>(MenuItem::currentMenuItem.top())){
		clearCli();
		cout << invalidMessage(size) << "\n";
		cout << "\nCurrent Request: " << MenuItem::transaction->getAmount() << " to " << MenuItem::transaction->getRecipientUserName() << "\n\n";
		cout << "Your Balance: " << MenuItem::user->getBalance() << "\n\n";

	}
	else {
		cout << invalidMessage(size) << "\n";
	}

}
void CLI::drawCli(bool isValid) {
	if (!dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem.top()) && !dynamic_cast<ViewRequestSettingsMenu*>(MenuItem::currentMenuItem.top())) {
		clearCli();
	}

	if (!isValid) {
		drawInvalid();
	}

	if (!dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem.top()) && !dynamic_cast<ViewRequestSettingsMenu*>(MenuItem::currentMenuItem.top())) {
		cout << "\nCurrent Menu: " + MenuItem::currentMenuItem.top()->getName() << "\n\n";
	}
	MenuItem::printMenu(MenuItem::currentMenuItem.top());

	string eofTerminal = "\033[9999H";
	cout << eofTerminal;

	cout << "Enter your choice: ";
}

int CLI::getInput(bool overwrite, int size, bool sorted) {
	
	int sizee = overwrite ? size : MenuItem::currentMenuItem.top()->getSubMenus().size();

	int choice;
	string input;

	cin >> input;

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
		return choice;
	}

	if ((tolower(choice) == 'r' || tolower(choice) == 'o') && sorted) {
		return choice;
	}


	return false;
	
}

