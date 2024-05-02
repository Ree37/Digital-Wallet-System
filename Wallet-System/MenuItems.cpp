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
	cout << "Input 'x' to leave menu\n\n";
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
				throw invalid_argument("Wrong Username or Password");
			}
			if (!BCryptLib::validatePassword(password, userData->getPassword())) {
				throw invalid_argument("Wrong Username or Password");
			}
		}
		catch (exception e) {
			CLI::clearCli();
			cout << "Input 'x' to leave menu\n\n";
			cout << e.what() << "\n\n";
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
	return true;
};

RegisterUserMenu::RegisterUserMenu(string name) : MenuItem(name) {};

bool RegisterUserMenu::update() {
	CLI::clearCli();

	string username, password;
	cout << "Input 'x' to leave menu\n";

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
			cout << "Input 'x' to leave menu\n\n";

			cout << e.what() << '\n';
			cout << "Enter Username: " << username << "\n";
		}
	}
	return true;
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

		cin >> recepientName;

		if (exitCommand(recepientName)) {
			return true;
		}

		cout << "Enter amount: ";
		cin >> input;

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
		cin >> input;

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
	bool isValid;

	isValid = true;
	while(true) {
		CLI::clearCli();
		if (!isValid)
		{
			cout << "Please enter valid amount (positive number)\n\n";
		}
		cout << "Enter amount: ";
		cin >> input;

		if (exitCommand(input))
		{
			return true;
		}
			
		try {
			amount = stof(input);
		}
		catch (exception e)
		{
			isValid = false;
			continue;
		}

		if (!user->addMoney(amount))
		{
			isValid = false;
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
		break;
		
	}

	return true;
};


ViewUserRequestsMenu::ViewUserRequestsMenu(string name) : MenuItem(name) {};
bool ViewUserRequestsMenu::update() {

	vector<Transaction*> v = user->getRequests();

	int size = v.size();

	int choice;
	bool isValid = true;
	do {
		
		CLI::clearCli();

		if (!isValid) {
			cout << CLI::invalidMessage(size) << "\n\n";
		}
		cout << "Current Menu: " << currentMenuItem.top()->name << "\n\n";
		if (!v.empty())
		{
			for (int i = 0; i < size; i++)
			{
				cout << i + 1 << "] " << *v[i] << '\n';

			}
		}
		else {
			cout << "No transactions found...\n";
			cout << "\nPress any key to go back..\n";

			while (!_kbhit()) {
			}
			_getch();
			back();
			return true;
		}

		string eofTerminal = "\033[9999H";
		cout << eofTerminal;

		cout << "Enter your choice: ";
		choice = CLI::getInput(true, size);

		if (choice == 0) {
			isValid = false;
		}
		else {
			isValid = true;
		}
	} while (!isValid);

	if (choice == 'x') {
		back();
		return true;
	}

	transaction = v[choice - 1];

	currentMenuItem.push(currentMenuItem.top()->getSubMenus().at(0));


}

ViewRequestSettingsMenu::ViewRequestSettingsMenu(string name) : MenuItem(name) {};
bool ViewRequestSettingsMenu::update() {
	CLI::clearCli();

	cout << "\nCurrent Request: " << transaction->getAmount() << " to " << transaction->getRecipientUserName() << "\n\n";
	cout << "Your Balance: " << MenuItem::user->getBalance() << "\n\n";

	MenuItem::update();

	return true;
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

	CLI::clearCli();
	cout << "Current Menu: " << currentMenuItem.top()->name << "\n\n";
	switch (mode)
	{
	case 1:	v = user->getSentTransactions();
			break;
	case 2:	v = user->getReceivedTransactions();
		break; 
	case 3:	v = user->getAllTransactions();
					 break;
	}

	if (!v.empty())
	{
		for (int i = 0; i < v.size(); i++)
		{
			cout << i + 1 << "] " << *v[i] << '\n';

		}
		cout << "Press any key to continue..\n";

		while (!_kbhit()) {
		}
		_getch();
	}
	else {
		cout << "No transactions found...\n";
	}

	back();
	return true;

}


ViewUserSentTransactionsMenu::ViewUserSentTransactionsMenu(string name) :ViewUserTransactionsMenu(name) { mode = 1; };

ViewUserRecievedTransactionsMenu::ViewUserRecievedTransactionsMenu(string name) : ViewUserTransactionsMenu(name) { mode = 2; };

ViewUserAllTransactionsMenu::ViewUserAllTransactionsMenu(string name) : ViewUserTransactionsMenu(name) { mode = 3; };

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


	CLI::clearCli();
	cout << "Input 'x' to leave menu\n\n";

	while (true) {
		cout << "Enter old password: ";
		cin >> oldPassword;

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
		catch (exception e){
			CLI::clearCli();
			cout << "Input 'x' to leave menu\n\n";
			cout << e.what() << "\n\n";
	
		}
	}

	while (true)
	{
		cout << "Enter new password: ";
		cin >> newPassword;

		if (exitCommand(newPassword))
		{
			return true;
		}

		try {
			if (!Utils::checkPasswordPolicy(newPassword)) {
				throw std::invalid_argument("Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long.");
			}

			string hash = BCryptLib::generateHash(newPassword, 12);
			user->setPassword(hash);

			cout << "\nPassword succesfully updated!\n";
			cout << "Press any key to continue..\n";

			while (!_kbhit()) {
			}
			_getch();

			back();
			back();
			break;
		}
		catch (exception e)
		{
			CLI::clearCli();
			cout << "Input 'x' to leave menu\n\n";
			cout << e.what() << "\n\n";
			cout << "Enter old password: " << oldPassword << "\n";

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

