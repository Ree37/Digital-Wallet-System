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

class AddMoneyMenu : public MenuItem
{
public:
	AddMoneyMenu(string name);
	bool update() override;

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

