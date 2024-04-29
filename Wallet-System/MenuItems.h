#pragma once

#include "CLI.h"
#include "User.h"
#include <stack>


class MenuItem {
public:
	MenuItem(string name);

	static void printMenu(MenuItem* menuItem);

	vector<MenuItem*> getSubMenus();
	string getName();
	void setName(string name);

	bool hasSubMenus();
	bool hasBack();
	void addSubMenu(MenuItem* subMenu);
	bool exitCommand(string s);

	virtual bool update();
	virtual bool back();

public:
	static User* user;
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

class ViewUserTransactionsMenu : public MenuItem
{
public:
	ViewUserTransactionsMenu(string name);
	bool update() override;

public:
	int mode;

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

class ChangeUserNameMenu : public MenuItem
{
public:
	ChangeUserNameMenu(string name);
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

