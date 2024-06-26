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
			currentMenuItem.top()->getSubMenus()[3]->setName("Register User");
		}
		return back();;
	}
	else {
		currentMenuItem.push(currentMenuItem.top()->getSubMenus()[choice - 1]);
	}
	return true;
}

template <typename T>
int MenuItem::updateList(vector<T*>& v, bool viewOnly, bool sorted) {


	int choice;
	int size;
	bool isValid = true;
	bool ascending = true;
	while(true) {
		size = v.size();

		CLI::clearCli();

		if(!viewOnly && sorted)
			cout << "Enter 'a' for ascending or 'd' for descending\n\n";

		if (!isValid) {
			cout << CLI::invalidMessage(size, sorted) << "\n\n";
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

			_getch();
			return 0;
		}

		if (viewOnly)
		{
			cout << "Press 'a' to view by ascending, 'd' to view by descending, any other key to go back..\n";

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
	    CLI::getInput(username);
		
		if (exitCommand(username)) {
			back();
			return true;
		}

		cout << "Enter Password: ";
		CLI::getPassword(password);

		if (exitCommand(password)) {
			back();
			return true;
		}

		User* data = (Container::admin->getUsername() == username ? Container::admin : Container::getUser(username));

	
		try {
			if (!data) {
				throw invalid_argument("Wrong Username or Password");
			}
			
			if (!BCryptLib::validatePassword(password, data->getPassword())) {
				throw invalid_argument("Wrong Username or Password");
			}
		
			
		}
		catch (const exception& e) {
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
		    CLI::getInput(i_otp);

			if (exitCommand(i_otp)) {
				back();
				return true;
			}

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
		CLI::getInput(username);

		if (exitCommand(username)) {
			back();
			return true;
		}

		try {
			Container::checkValidUser(username);
			break;
		}
		catch(const exception& e){
			CLI::clearCli();
			cout << "Input 'x' to leave register menu. Press space to toggle password visibility\n\n";
			cout << e.what() << '\n';
		}
	}
	

	while (true) {
		try {
			cout << "Enter Password: ";
			CLI::getPassword(password);

			if (exitCommand(password)) {
				back();
				return true;
			}

			if (!Utils::checkPasswordPolicy(password))
			{
				throw std::invalid_argument("Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long.");
			}

			cout << "Confirm Password: ";
			CLI::getPassword(confirm);

			if (exitCommand(confirm)) {
				back();
				return true;
			}

			if (password != confirm)
			{
				throw std::invalid_argument("Password not same as Confirm password.");
			}
			
			string hash = BCryptLib::generateHash(password, 12);

			MenuItem::user = new User(username, hash);

			Container::addUser(MenuItem::user);

			if(admin){
				cout << "\nUser Added Sucessfully. Press any key to go back...";

				_getch();

				back();

				break;
			}

			currentMenuItem.push(currentMenuItem.top()->getSubMenus()[0]);

			break;
		}
		catch (const exception& e) {
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
	if (user->getSuspendedFlag())
		cout << "You are currently suspended from making any transactions\n";
	cout << "\nCurrent User: " << MenuItem::user->getUsername() << "\n\n";
	cout << "Available Balance: " << MenuItem::user->getBalance() << " EGP" << "\n\n";

	if(!user->getSuspendedFlag()) {
		user->getToRequests().size() != 1 ? cout << "You have " << user->getToRequests().size() << " Requests\n\n" : cout << "You have " << user->getToRequests().size() << " Request\n\n";
	}
}

bool UserProfileMenu::back() {
	currentMenuItem.pop();
	currentMenuItem.pop();

	return true;
};

TransferMoneyMenu::TransferMoneyMenu(string name) : MenuItem(name) {};

bool TransferMoneyMenu::update() {
	string input;
	double amount;
	

	CLI::clearCli();
	cout << "Input 'x' to leave menu\n\n";

	string recepientName;

	while (true) {

		cout << "Enter " << (isSend ? "recipent" : "requested") << " Username: ";

		CLI::getInput(recepientName);

		if (exitCommand(recepientName)) {
			back();
			return true;
		}

		cout << "Enter amount: ";
		CLI::getInput(input);

		if (exitCommand(input)){
			back();
			return true;
		}
		

		try {

			try {
				amount = stod(input);
			}
			catch (const exception& e)
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
			cout << "You succesfully " << (isSend ? "sent " : "requested ") << round(amount*100)/100 << " EGP " << (isSend ? "to " : "from ") << recepientName << "..\n\n";
			cout << "Press any key to continue..\n";
			_getch();
			back();
			break;
		}
		catch (const exception& e)
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
		CLI::getCreditCard(input);

		if (exitCommand(input)){
			back();
			return true;
		}


		long long cc;
		try {
			try {
				cc = std::stoll(input);
			}
			catch (const exception& e)
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
		catch (const exception& e) {
			CLI::clearCli();
			cout << "Input 'x' to leave menu\n\n";
			cout << e.what() << "\n\n";
		}
	}

	
	
	string input;
	double amount;
	CLI::clearCli();
	cout << "Input 'x' to leave menu\n\n";

	while(true) {
		cout << "Enter amount: ";
		CLI::getInput(input);

		if (exitCommand(input)){
			back();
			return true;
		}
			
		try {
			try {
				amount = stod(input);
			}
			catch (const exception& e)
			{
				throw invalid_argument("Please Enter a number");
			}
			user->addMoney(amount);
		}
		catch (const exception& e)
		{
			CLI::clearCli();
			cout << "Input 'x' to leave menu\n\n";

			cout << e.what() << "\n\n";
			continue;
		}

		string card;

		switch (type)
		{
		case 1: card = "MEEZA"; break;
		case 2: card = "MASTERCARD"; break;
		case 3: card = "VISA"; 
		}

		cout << "\nYou succesfully added " << round(amount*100)/100 << " EGP through a " << card << " card\n";

		cout << "\npress any key to continue..";

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
	cout << "\n\nCurrent Request: " << transaction->getAmount() << " EGP to " << transaction->getRecipientUserName() << "\n\n";
	cout << "Your Balance: " << MenuItem::user->getBalance() << " EGP" << "\n\n";
}


AcceptRequestMenu::AcceptRequestMenu(string name) : MenuItem(name) {};
bool AcceptRequestMenu::update() {

	CLI::clearCli();
	try{
		transaction->acceptTransaction();

		cout << "You succesfully sent " << transaction->getAmount() << " EGP to " << transaction->getRecipientUserName() <<"..\n\n";
		cout << "Press any key to continue..\n";
		

		_getch();
		back();
		back();
		return true;

	}
	catch (const exception& e){
		cout << "Transaction failed: " << e.what() << "\n";
		cout << "Press any key to continue..\n";
		_getch();
		back();
		return true;
	}


}

DeclineRequestMenu::DeclineRequestMenu(string name) : MenuItem(name) {};

bool DeclineRequestMenu::update() {
	CLI::clearCli();

	try {
		transaction->declineTransaction();

		cout << "Request declined..\n";
		cout << "Press any key to continue..\n";


		_getch();
		back();
		back();
		return true;

	}
	catch (const exception& e) {
		cout << "Transaction failed: " << e.what() << "\n";
		cout << "Press any key to continue..\n";
		_getch();
		back();
		return true;
	}


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
	bool pwdVisible;


	CLI::clearCli();
	cout << "Input 'x' to leave menu. Press space to toggle password visibility\n\n";

	if (!admin)
	{
		while (true) {
			cout << "Enter old password: ";
			pwdVisible = CLI::getPassword(oldPassword);

			if (exitCommand(oldPassword)){
				back();
				return true;
			}

			try {
				if (!BCryptLib::validatePassword(oldPassword, user->getPassword())) {
					throw invalid_argument("Wrong Password");
				}

				break;
			}
			catch (const exception& e) {
				CLI::clearCli();
				cout << "Input 'x' to leave menu. Press space to toggle password visibility\n\n";
				cout << e.what() << "\n\n";

			}
		}
	}
	

	while (true)
	{
		cout << "Enter new password: ";
		CLI::getPassword(newPassword);

		if (exitCommand(newPassword)){
			back();
			return true;
		}

		try {
			if (!Utils::checkPasswordPolicy(newPassword)) {
				throw std::invalid_argument("Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long.");
			}

			cout << "Confirm password: ";
			CLI::getPassword(confirm);

			if (exitCommand(confirm)) {
				back();
				return true;
			}

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

			_getch();

			back();
			break;
		}
		catch (const exception& e)
		{
			CLI::clearCli();
			cout << "Input 'x' to leave menu. Press space to toggle password visibility\n\n";
			cout << e.what() << "\n\n";
			if(!admin)
				cout << "Enter old password: " << (pwdVisible ? oldPassword : std::string(oldPassword.size(), '*')) << '\n';

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
		cout << "Input 'x' to leave menu\n\n";
		cout << "Enter 2FA Key: ";

		while (true) {
			try {
				CLI::getInput(i_otp);

				if (exitCommand(i_otp)) {
					back();
					return true;
				}

				unsigned int otp = TOTPLib::getOTP(secret);

				if (stoi(i_otp) != otp)
				{
					throw invalid_argument("Invalid 2FA Key");
				}

				CLI::clearCli();
				cout << "Press any key to go back\n\n";

				user->setIsHas2FA(false);
				user->setTotpSecret("");

				cout << "2FA has been disabled!\n";

				_getch();
				back();

				return true;
			}
			catch(const exception& e){
				CLI::clearCli(); 
				cout << e.what() << "\n\n";
				cout << "Input 'x' to leave menu\n\n";
				cout << "Enter 2FA Key: ";

			}
		}
	}
	
	string password;
	cout << "Input 'x' to leave 2FA menu\n\n";
	cout << "Enter Current Password to enable 2FA: ";

	while (true) {
		try {
			CLI::getPassword(password);

			if (exitCommand(password)) {
				back();
				return true;
			}

			if (!BCryptLib::validatePassword(password, user->getPassword())) {
				throw invalid_argument("Wrong Password");
			}

			currentMenuItem.push(currentMenuItem.top()->getSubMenus()[0]);
			return true;
		}
		catch (const exception& e) {
			CLI::clearCli();
			cout << e.what() << "\n\n";
			cout << "Input 'x' to leave 2FA menu\n\n";
			cout << "Enter Current Password to enable 2FA: ";
		}
	}
}

QrCodeMenu::QrCodeMenu(string name) : MenuItem(name) {};

bool QrCodeMenu::update() {
	
	CLI::clearCli();

	if (user->getTotpSecret().empty())
		user->setTotpSecret(TOTPLib::generateSecret());

	QrcodeLib* qrcode = new QrcodeLib(user->getUsername(), user->getTotpSecret());

	qrcode->printQRCodeASCII();
	cout << "\nPress 'x' to go back or any other key to Enter your TOTP\n\n";

	int width = Utils::getConsoleWidth();
	int new_width;

	
	while (!_kbhit())
	{
		new_width = Utils::getConsoleWidth();

		if (new_width != width)
		{
			CLI::clearCli();
			qrcode->printQRCodeASCII();
			cout << "\nPress 'x' to go back or any other key to Enter your TOTP\n\n";
			width = new_width;
		}

		Sleep(300);
	}
	char c = _getch();
	if (exitCommand(string(1, c))) {
		user->setTotpSecret("");
		back();
		back();
		return true;
	}

	currentMenuItem.push(currentMenuItem.top()->getSubMenus()[0]);
	return true;
	

}

Confirm2FAMenu::Confirm2FAMenu(string name) : MenuItem(name) {};

bool Confirm2FAMenu::update() {
	CLI::clearCli();
	cout << "\nEnter 'x' to go back or your TOTP to confirm Two-Factor-Authentication\n";
	while (true) {
		string totp;

		CLI::getInput(totp);

		if (exitCommand(totp)) {
			back();
			return true;
		}

		try{

			try {
				if (stoi(totp) != TOTPLib::getOTP(user->getTotpSecret())) {
					throw exception();
				}
			}
			catch (const exception& e) {
				throw invalid_argument("Enter a valid TOTP");
			}
			

			cout << "\n\nTwo-Factor-Authentication Enabled Succesfully. Press any key to continue...";

			_getch();

			user->setIsHas2FA(true);
			back();
			back();
			back();
			return true;

		}
		catch (const exception& e)
		{
			CLI::clearCli();
			cout << e.what() << '\n';
			cout << "\nEnter 'x' to go back or your TOTP to confirm Two-Factor-Authentication\n";

		}

	}
}


AdminProfile::AdminProfile(string name) : MenuItem(name) {};

void AdminProfile::customHeader() {
	currentMenuItem.top()->getSubMenus()[3]->setName("Add User");
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


a_SearchUser::a_SearchUser(string name) : MenuItem(name) {};

bool a_SearchUser::update() {

	CLI::clearCli();

	cout << "\nEnter user name to search: ";

	string username;
	CLI::getInput(username);

	if (exitCommand(username)) {
		back();
		return true;
	}

	vector<User*> v = admin->search(username);

	while (true) {

		int state = updateList(v, false, false);

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
	cout << "User Balance: "  << MenuItem::user->getBalance() << " EGP" << "\n\n";
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
		CLI::getInput(input);
		if (exitCommand(input)){
			back();
			return true;
		}
		double balance;
		try {
			try {
				balance = stod(input);
			}
			catch (const exception& e)
			{
				throw invalid_argument("Please Enter a number.");
			}

			admin->adjustUserBalance(user->getUsername(), balance);
			back();
			return true;

		}
		catch (const exception& e)
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

	_getch();

	back();
	return true;

}

a_DeleteUser::a_DeleteUser(string name) : MenuItem(name) {};

bool a_DeleteUser::update() {

	CLI::clearCli();
	cout << "Are you sure you want to PERMANENTLY DELETE this user? Y/y for yes, any other input to go back..\n";
	string input;
	CLI::getInput(input);

	if (input.size() == 1 && tolower(input[0]) == 'y') {
		admin->deleteUser(user->getUsername());
		cout << "\nYou deleted this user. Press any key to go back..";
		_getch();

		back();

	}
	back();
	return true;
}



