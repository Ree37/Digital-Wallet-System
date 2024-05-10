#pragma once
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
Admin* MenuItem::admin;
Transaction* MenuItem::transaction;

stack <MenuItem*> MenuItem::currentMenuItem;


MenuItem::MenuItem(std::string name) : name(name) {}

void MenuItem::printMenu(MenuItem* menuItem) {
	for (size_t i = 0; i < menuItem->getSubMenus().size(); i++) {
		cout << i + 1 << "] " << menuItem->getSubMenus().at(i)->name << "\n";
	}

	if (dynamic_cast<MainMenu*>(MenuItem::currentMenuItem.top())) {
		cout << "x] Exit" << "\n";
	}
	else if (dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem.top()) || dynamic_cast<AdminProfile*>(MenuItem::currentMenuItem.top())) {
		cout << "x] Log Out" << "\n";
	}
	else {
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


void MenuItem::addSubMenu(MenuItem* subMenu) {
	subMenus.push_back(subMenu);
}

void MenuItem::customHeader(){
	cout << "\nCurrent Menu: " + MenuItem::currentMenuItem.top()->getName() << "\n\n";
}

bool MenuItem::update() {
	bool isValid = true;
	int choice;
	do {
		CLI::drawCli(isValid);
		choice = CLI::getChoice();

		if (choice == 0) {
			isValid = false;
		}
		else {
			isValid = true;
		}
	} while (!isValid);

	if (choice == 'x') {
		if (dynamic_cast<AdminProfile*>(currentMenuItem.top())) {
			currentMenuItem.top()->getSubMenus()[2]->setName("Register User");
		}
		return back();;
	}
	else {
		currentMenuItem.push(currentMenuItem.top()->getSubMenus()[choice - 1]);
	}
	return true;
}

template <typename T>
int MenuItem::updateList(vector<T*>& v, bool viewOnly) {


	int choice;
	int size;
	bool isValid = true;
	bool ascending = true;
	while(true) {
		size = v.size();

		CLI::clearCli();

		if(!viewOnly)
			cout << "Enter 'a' for ascending or 'd' for descending\n\n";

		if (!isValid) {
			cout << CLI::invalidMessage(size, true) << "\n\n";
		}
		cout << "Current Menu: " << currentMenuItem.top()->name << "\n\n";
		if (!v.empty())
		{
			int i = 0;
			if (ascending) {
				for (auto it = v.begin(); it != v.end(); ++it) {
					cout << i + 1 << "] " << **it << "\n";
					i++;
				}
			}
			else {
				for (auto it = v.rbegin(); it != v.rend(); ++it) {
					cout << i + 1 << "] " << **it << "\n";
					i++;
				}
			}
			
		}
		else {
			cout << "No result found...\n";
			cout << "\nPress any key to go back..\n";

			while (!_kbhit()) {
			}
			_getch();
			return 0;
		}

		if (viewOnly)
		{
			cout << "Press 'a' to view by ascending, 'd' to view by descending, any other key to go back..\n";

			while (!_kbhit()) {
			}
			char c = _getch();

			if (tolower(c) == 'a')
			{
				ascending = true;
				continue;
			}
			if (tolower(c) == 'd')
			{
				ascending = false;
				continue;
			}

			return 0;
		}

		cout << "x] Back";
		string eofTerminal = "\033[9999H";
		cout << eofTerminal;


		cout << "Enter your choice: ";
		choice = CLI::getChoice(true, size, true);

		isValid = true;

		if (choice == 0) {
			isValid = false;
			continue;
		}
		if (tolower(choice) == 'a')
		{
			ascending = true;
			continue;
		}
		if (tolower(choice) == 'd')
		{
			ascending = false;
			continue;
		}
		if (choice == 'x') {
			return 0;
		}
		return (ascending ? choice : size - choice + 1);
	}

	

	
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
	cout << "Input 'x' to leave login menu. Press space to toggle password visibility\n\n";
	while (true) {
		string username, password;

		cout << "Enter Username: ";
		username = CLI::getInput();
		
		if (exitCommand(username))
			return true;

		cout << "Enter Password: ";
		password = CLI::getPassword();

		if (exitCommand(password))
			return true;

		User* data = (Container::admin->getUsername() == username ? Container::admin : Container::getUser(username));

	
		try {
			if (!data) {
				throw invalid_argument("Wrong Username or Password");
			}
			
			if (!BCryptLib::validatePassword(password, data->getPassword())) {
				throw invalid_argument("Wrong Username or Password");
			}
		
			
		}
		catch (exception e) {
			CLI::clearCli();
			cout << "Input 'x' to leave login menu. Press space to toggle password visibility\n\n";
			cout << e.what() << "\n\n";
			continue;
		}

		
		if (data->getIsHas2FA())
		{
			string i_otp;
			string secret = data->getTotpSecret();
			QrcodeLib* qrcode = new QrcodeLib(data->getUsername(), secret);
			cout << "Enter 2FA Key: ";
			i_otp = CLI::getInput();

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

		if(!dynamic_cast<Admin*>(data)) {

			MenuItem::user = data;
			MenuItem::admin = nullptr;
			currentMenuItem.push(currentMenuItem.top()->getSubMenus()[0]);

		}
		else {
			MenuItem::user = nullptr;
			MenuItem::admin = dynamic_cast<Admin*>(data);
			currentMenuItem.push(currentMenuItem.top()->getSubMenus()[1]);
		}
		break;
	}
	return true;
};

RegisterUserMenu::RegisterUserMenu(string name) : MenuItem(name) {};

bool RegisterUserMenu::update() {
	CLI::clearCli();

	string username, password, confirm;

	cout << "Input 'x' to leave register menu. Press space to toggle password visibility\n\n";
	while (true) {
		

		cout << "Enter Username: ";
		username = CLI::getInput();

		if (exitCommand(username))
			return true;

		try {
			Container::checkUniqueUser(username);
			break;
		}
		catch(exception e){
			CLI::clearCli();
			cout << "Input 'x' to leave register menu. Press space to toggle password visibility\n\n";
			cout << e.what() << '\n';
		}
	}
	

	while (true) {
		try {
			cout << "Enter Password: ";
			password = CLI::getPassword();

			if (exitCommand(password))
				return true;

			if (!Utils::checkPasswordPolicy(password))
			{
				throw std::invalid_argument("Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long.");
			}

			cout << "Confirm Password: ";
			confirm = CLI::getPassword();

			if (exitCommand(confirm))
				return true;

			if (password != confirm)
			{
				throw std::invalid_argument("Password not same as Confirm password.");
			}
			
			string hash = BCryptLib::generateHash(password, 12);

			MenuItem::user = new User(username, hash);

			Container::addUser(MenuItem::user);

			if(admin){
				cout << "\nUser Added Sucessfully. Press any key to go back...";

				while(!_kbhit()){}
				_getch();

				back();

				break;
			}

			currentMenuItem.push(currentMenuItem.top()->getSubMenus()[0]);

			break;
		}
		catch (exception e) {
			CLI::clearCli();
			cout << "Input 'x' to leave register menu. Press space to toggle password visibility\n\n";

			cout << e.what() << '\n';
			cout << "Enter Username: " << username << "\n";
		}
	}
	return true;
};


UserProfileMenu::UserProfileMenu(string name) : MenuItem(name) {};

void UserProfileMenu::customHeader() {
	cout << "\nCurrent User: " << MenuItem::user->getUsername() << "\n\n";
	cout << "Available Balance: " << MenuItem::user->getBalance() << "\n\n";
}

bool UserProfileMenu::back() {
	currentMenuItem.pop();
	currentMenuItem.pop();

	return true;
};

TransferMoneyMenu::TransferMoneyMenu(string name) : MenuItem(name) {};

bool TransferMoneyMenu::update() {
	string input;
	float amount;
	string state;
	if (isSend)
	{
		state = "recepient";
	}
	else {
		state = "requested";
	}

	CLI::clearCli();
	cout << "Input 'x' to leave menu\n\n";

	string recepientName;

	while (true) {

		cout << "Enter " << state << " Username: ";

		recepientName = CLI::getInput();

		if (exitCommand(recepientName)) {
			return true;
		}

		cout << "Enter amount: ";
		input = CLI::getInput();

		if (exitCommand(input))
		{
			return true;
		}
		

		try {

			try {
				amount = stof(input);
			}
			catch (exception e)
			{
				throw invalid_argument("Please enter a number");
			}

			if (isSend)
			{
				user->makeTransaction(recepientName, amount);
			}
			else {
				user->makeRequest(recepientName, amount);
			}
			back();
			break;
		}
		catch (exception e)
		{
			CLI::clearCli();
			cout << "Input 'x' to leave menu\n\n";
			cout << e.what() << "\n\n";
		}
	}
	return true;

	}
	

SendMoneyMenu::SendMoneyMenu(string name) : TransferMoneyMenu(name) { isSend = true; };

RequestMoneyMenu::RequestMoneyMenu(string name) : TransferMoneyMenu(name) { isSend = false; };

AddMoneyMenu::AddMoneyMenu(string name) : MenuItem(name) {};

bool AddMoneyMenu::update() {


	CLI::clearCli();
	cout << "Input 'x' to leave menu\n";
	int type;
	while (true) {
		cout << "Enter Credit Card (16 numbers): ";
		string input;
		input = CLI::getInput();

		if (exitCommand(input))
		{
			return true;
		}


		long long cc;
		try {
			try {
				cc = std::stoll(input);
			}
			catch (exception e)
			{
				throw invalid_argument("Please enter a number");
			}

			type = Utils::checkCreditCard(cc);

			if (type == -1)
			{
				throw invalid_argument("Please enter a valid credit card number");
			}
			break;
		}
		catch (exception e) {
			CLI::clearCli();
			cout << "Input 'x' to leave menu\n\n";
			cout << e.what() << "\n\n";
		}
	}

	
	
	string input;
	float amount;
	CLI::clearCli();
	while(true) {
		cout << "Enter amount: ";
		input = CLI::getInput();

		if (exitCommand(input))
		{
			return true;
		}
			
		try {
			amount = stof(input);
			user->addMoney(amount);
		}
		catch (exception e)
		{
			CLI::clearCli();
			cout << e.what() << '\n';
			continue;
		}

		string card;

		switch (type)
		{
		case 1: card = "AMERICAN EXPRESS"; break;
		case 2: card = "MASTERCARD"; break;
		case 3: card = "VISA"; 
		}

		cout << "\nYou succesfully added " << amount << " through a " << card << " card\n";

		cout << "\npress any key to continue..";

		while (!_kbhit()) {
		}
		_getch();
		back();
		return true;

	}

};

ViewRequestsMenu::ViewRequestsMenu(string name) : MenuItem(name) {};

bool ViewRequestsMenu::update(){
	if (admin) {
		currentMenuItem.top()->getSubMenus()[0]->setName("Requests to " + user->getUsername());
		currentMenuItem.top()->getSubMenus()[1]->setName("Requests from " + user->getUsername());
	}
	else {
		currentMenuItem.top()->getSubMenus()[0]->setName("Requests to you");
		currentMenuItem.top()->getSubMenus()[1]->setName("Requests from you");
	}

	MenuItem::update();
	return true;
}


ViewToUserRequestsMenu::ViewToUserRequestsMenu(string name) : ViewUserTransactionsMenu(name) { mode = toRequest; };

ViewFromUserRequestsMenu::ViewFromUserRequestsMenu(string name) : ViewUserTransactionsMenu(name) { mode = fromRequest; };

ViewRequestSettingsMenu::ViewRequestSettingsMenu(string name) : MenuItem(name) {};

void ViewRequestSettingsMenu::customHeader() {
	cout << "\n\nCurrent Request: " << transaction->getAmount() << " to " << transaction->getRecipientUserName() << "\n\n";
	cout << "Your Balance: " << MenuItem::user->getBalance() << "\n\n";
}


AcceptRequestMenu::AcceptRequestMenu(string name) : MenuItem(name) {};
bool AcceptRequestMenu::update() {

	CLI::clearCli();
	try{
		transaction->acceptTransaction();

		cout << "You succesfully sent " << transaction->getAmount() << " to " << transaction->getRecipientUserName() <<"..\n\n";
		cout << "Press any key to continue..\n";
		

		while(!_kbhit()){}
		_getch();
		back();
		back();
		return true;

	}
	catch (exception e){
		cout << "Transaction failed: " << e.what() << "\n";
		cout << "Press any key to continue..\n";
		while (!_kbhit()) {}
		_getch();
		back();
		return true;
	}


}

DeclineRequestMenu::DeclineRequestMenu(string name) : MenuItem(name) {};

bool DeclineRequestMenu::update() {
	CLI::clearCli();
	
	transaction->declineTransaction();

	cout << "Request declined..\n";
	cout << "Press any key to continue..\n";


	while (!_kbhit()) {}
	_getch();
	back();
	back();
	return true;


}


ViewUserTransactionsMenu::ViewUserTransactionsMenu(string name) : MenuItem(name) {};
bool ViewUserTransactionsMenu::update() {


	vector<Transaction*> v;

	switch (mode)
		{
		case sentUser: v = user->getSentTransactions();
			break;
		case recievedUser: v = user->getReceivedTransactions();
			break;
		case allUser: v = user->getAllTransactions();
			break;
		case toRequest: v = user->getToRequests();
			break;
		case fromRequest :v = user->getFromRequests();
			break;
		case allAdmin: v = admin->viewAllUsersTransactions();
		}

	bool isViewOnly = (admin || mode != toRequest);

	while (true) {
		


		int state = updateList(v, isViewOnly);

		if (state == 0)
		{
			back(); 
			return true;
		}

		transaction = v[state - 1];
		currentMenuItem.push(currentMenuItem.top()->getSubMenus().at(0));
		return true;
		
	}

}


ViewUserSentTransactionsMenu::ViewUserSentTransactionsMenu(string name) :ViewUserTransactionsMenu(name) { mode = sentUser; };

ViewUserRecievedTransactionsMenu::ViewUserRecievedTransactionsMenu(string name) : ViewUserTransactionsMenu(name) { mode = recievedUser; };

ViewUserAllTransactionsMenu::ViewUserAllTransactionsMenu(string name) : ViewUserTransactionsMenu(name) { mode = allUser; };

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

bool ChangePasswordMenu::update() {

	string oldPassword;
	string newPassword;
	string confirm;


	CLI::clearCli();
	cout << "Input 'x' to leave menu. Press space to toggle password visibility\n\n";

	if (!admin)
	{
		while (true) {
			cout << "Enter old password: ";
			oldPassword = CLI::getPassword();

			if (exitCommand(oldPassword))
			{
				return true;
			}

			try {
				if (!BCryptLib::validatePassword(oldPassword, user->getPassword())) {
					throw invalid_argument("Wrong Password");
				}

				break;
			}
			catch (exception e) {
				CLI::clearCli();
				cout << "Input 'x' to leave menu. Press space to toggle password visibility\n\n";
				cout << e.what() << "\n\n";

			}
		}
	}
	

	while (true)
	{
		cout << "Enter new password: ";
		newPassword = CLI::getPassword();

		if (exitCommand(newPassword))
		{
			return true;
		}

		try {
			if (!Utils::checkPasswordPolicy(newPassword)) {
				throw std::invalid_argument("Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long.");
			}

			cout << "Confirm Password: ";
			confirm = CLI::getPassword();

			if (exitCommand(confirm))
				return true;

			if (newPassword != confirm)
			{
				throw std::invalid_argument("Password not same as Confirm password.");
			}


			string hash = BCryptLib::generateHash(newPassword, 12);
			if (admin) {
				admin->editUser(user->getUsername(), hash);
			}
			else {
				user->setPassword(hash);
			}

			cout << "\nPassword succesfully updated!\n";
			cout << "Press any key to continue..\n";

			while (!_kbhit()) {
			}
			_getch();

			back();
			break;
		}
		catch (exception e)
		{
			CLI::clearCli();
			cout << "Input 'x' to leave menu. Press space to toggle password visibility\n\n";
			cout << e.what() << "\n\n";

		}
	}


	return true;
};

Enable2FAMenu::Enable2FAMenu(string name) : MenuItem(name) {};

bool Enable2FAMenu::update() {
	CLI::clearCli();

	if (user->getIsHas2FA())
	{
		std::string i_otp;

		string secret = user->getTotpSecret();
		QrcodeLib* qrcode = new QrcodeLib(user->getUsername(), secret);
		cout << "Input 'x' to leave menu\n";
		cout << "Enter 2FA Key: ";
		i_otp = CLI::getInput();

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
	password = CLI::getInput();

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



AdminProfile::AdminProfile(string name) : MenuItem(name) {};

void AdminProfile::customHeader() {
	currentMenuItem.top()->getSubMenus()[2]->setName("Add User");
	cout << "\nCurrent Admin: " << admin->getUsername() << "\n\n";
}

bool AdminProfile::back() {
	currentMenuItem.pop();
	currentMenuItem.pop();
	return true;
}

a_AllTransactions::a_AllTransactions(string name) : ViewUserTransactionsMenu(name) { mode = allAdmin; };

a_AllUsers::a_AllUsers(string name) : MenuItem(name) {};

bool a_AllUsers::update() {

	vector<User*> v = admin->viewUsers();
	while (true) {

		int state = updateList(v, false);

		if (state == 0)
		{
			back();
			return true;
		}

		user = v[state - 1];
		currentMenuItem.push(currentMenuItem.top()->getSubMenus()[0]);
		return true;

	}
}
a_ModifyUserProfile::a_ModifyUserProfile(string name) : MenuItem(name) {};

void a_ModifyUserProfile::customHeader() {
	cout << "\nCurrent User: " << user->getUsername() << '\n';
	cout << "User Balance: " << MenuItem::user->getBalance() << "\n\n";
}

bool a_ModifyUserProfile::update() {

	if (user->getSuspendedFlag())
	{
		currentMenuItem.top()->getSubMenus()[4]->setName("Unsuspend User");
	}
	else
	{
		currentMenuItem.top()->getSubMenus()[4]->setName("Suspend User");

	}
	MenuItem::update();
	return true;
}


a_SetBalance::a_SetBalance(string name) : MenuItem(name) {};

bool a_SetBalance::update() {
	CLI::clearCli();

	while (true) {
		cout << "Enter Amount to Set User's balance to:\n";
		string input;
		input = CLI::getInput();
		float balance;
		try {
			try {
				balance = stof(input);
			}
			catch (exception e)
			{
				throw invalid_argument("Please Enter a number.");
			}

			admin->adjustUserBalance(user->getUsername(), balance);
			back();
			return true;

		}
		catch (exception e)
		{
			CLI::clearCli();
			cout << e.what() << '\n';
		}
	}


}

a_SuspendUser::a_SuspendUser(string name) : MenuItem(name) {};

bool a_SuspendUser::update() {
	
	CLI::clearCli();

	admin->setSuspendUsers(user->getUsername());

	if (user->getSuspendedFlag()) {
		cout << "User Suspended...\n";
	}
	else {
		cout << "User Unsuspended..\n";
	}

	cout << "Press any key to go back..";

	while(!_kbhit()){}

	_getch();

	back();
	return true;

}

a_DeleteUser::a_DeleteUser(string name) : MenuItem(name) {};

bool a_DeleteUser::update() {

	CLI::clearCli();
	cout << "Are you sure you want to PERMANENTLY DELETE this user? Y/y for yes, any other input to go back..\n";
	string input;
	input = CLI::getInput();

	if (input.size() == 1 && tolower(input[0]) == 'y') {
		admin->deleteUser(user->getUsername());
		cout << "\nYou deleted this user. Press any key to go back..";
		while(!_kbhit()){}
		_getch();

		back();

	}
	back();
	return true;
}



