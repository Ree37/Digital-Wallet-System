#pragma once

#include "CLI.h"
#include "User.h"
#include "Admin.h"
#include <stack>

class MenuItem {
public:
	MenuItem(string name);

	static void printMenu(MenuItem* menuItem);

	vector<MenuItem*> getSubMenus();
	string getName();
	void setName(string name);

	void addSubMenu(MenuItem* subMenu);
	bool exitCommand(string s);

	virtual bool update();
	virtual bool back();

	template <typename T>
	int updateList(vector<T*> &v, bool viewOnly);

public:
	static User* user;
	static Transaction* transaction;
	static stack<MenuItem*> currentMenuItem;
	string name;
	vector<MenuItem*> subMenus;
};

class MainMenu : public MenuItem
{
public:
	MainMenu(string name);
	bool back() override;
};

class LoginUserMenu : public MenuItem
{
public:
	LoginUserMenu(string name);
	bool update() override;
};

class RegisterUserMenu : public MenuItem
{
public:
	RegisterUserMenu(string name);
	bool update() override;
};

class UserProfileMenu : public MenuItem
{
public:
	UserProfileMenu(string name);

	bool back() override;
	bool update() override;
};

class TransferMoneyMenu : public MenuItem
{
public:
	TransferMoneyMenu(string name);
	bool update() override;
public:
	bool isSend;
};

class SendMoneyMenu : public TransferMoneyMenu
{
public:
	SendMoneyMenu(string name);
};

class RequestMoneyMenu : public TransferMoneyMenu
{
public:
	RequestMoneyMenu(string name);
};

class AddMoneyMenu : public MenuItem
{
public:
	AddMoneyMenu(string name);
	bool update() override;

};

class ViewToUserRequestsMenu : public MenuItem
{
public:
	ViewToUserRequestsMenu(string name);
	bool update() override;
};

class ViewRequestSettingsMenu : public MenuItem
{
public:
	ViewRequestSettingsMenu(string name);
	bool update() override;
};

class AcceptRequestMenu : public MenuItem
{
public:
	AcceptRequestMenu(string name);
	bool update() override;

};
class DeclineRequestMenu : public MenuItem
{
public:
	DeclineRequestMenu(string name);
	bool update() override;

};

class ViewUserTransactionsMenu : public MenuItem
{
public:
	ViewUserTransactionsMenu(string name);
	bool update() override;

public:
	int mode;

};

class ViewFromUserRequestsMenu : public ViewUserTransactionsMenu
{
public:
	ViewFromUserRequestsMenu(string name);
};

class ViewUserSentTransactionsMenu : public ViewUserTransactionsMenu
{
public:
	ViewUserSentTransactionsMenu(string name);
};
class ViewUserRecievedTransactionsMenu : public ViewUserTransactionsMenu
{
public:
	ViewUserRecievedTransactionsMenu(string name);
};
class ViewUserAllTransactionsMenu : public ViewUserTransactionsMenu
{
public:
	ViewUserAllTransactionsMenu(string name);
};



class SettingsMenu : public MenuItem
{
public:
	SettingsMenu(string name);
	bool update() override;
};


class ChangePasswordMenu : public MenuItem
{
public:
	ChangePasswordMenu(string name);
	bool update() override;
};

class Enable2FAMenu : public MenuItem
{
public:
	Enable2FAMenu(string name);
	bool update() override;
};

class AdminProfile : public MenuItem
{
public:
	AdminProfile(string name);
	bool back() override;
};

class AllTransactions : public MenuItem
{
public:
	AllTransactions(string name);
	bool update() override;
};

class AllUsers : public MenuItem
{
public:
	AllUsers(string name);
	bool update() override;
};