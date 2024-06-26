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
	int updateList(vector<T*>& v, bool viewOnly, bool sorted = true);

	virtual void customHeader();

public:
	static User* user;
	static Admin* admin;
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
	void customHeader() override;
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

class ViewRequestsMenu : public MenuItem
{
public:
	ViewRequestsMenu(string name);
	bool update() override;

};

enum TransactionType {
	sentUser,
	recievedUser,
	allUser,
	toRequest,
	fromRequest,
	allAdmin
};

class ViewUserTransactionsMenu : public MenuItem
{
public:
	ViewUserTransactionsMenu(string name);
	bool update() override;

public:
	TransactionType mode;

};

class ViewToUserRequestsMenu : public ViewUserTransactionsMenu
{
public:
	ViewToUserRequestsMenu(string name);
};

class ViewRequestSettingsMenu : public MenuItem
{
public:
	ViewRequestSettingsMenu(string name);
	void customHeader() override;
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

class QrCodeMenu : public MenuItem
{
public:
	QrCodeMenu(string name);
	bool update() override;
};
class Confirm2FAMenu : public MenuItem
{
public:
	Confirm2FAMenu(string name);
	bool update() override;
};

class AdminProfile : public MenuItem
{
public:
	AdminProfile(string name);
	bool back() override;
	void customHeader() override;
};

class a_AllTransactions : public ViewUserTransactionsMenu
{
public:
	a_AllTransactions(string name);
};

class a_AllUsers : public MenuItem
{
public:
	a_AllUsers(string name);
	bool update() override;
};

class a_SearchUser : public MenuItem
{
public:
	a_SearchUser(string name);
	bool update() override;
};

class a_ModifyUserProfile : public MenuItem
{
public:
	a_ModifyUserProfile(string name);
	bool update() override;
	void customHeader() override;
};

class a_SetBalance : public MenuItem
{
public:
	a_SetBalance(string name);
	bool update() override;
};

class a_SuspendUser : public MenuItem
{
public:
	a_SuspendUser(string name);
	bool update() override;
};

class a_DeleteUser : public MenuItem
{
public:
	a_DeleteUser(string name);
	bool update() override;
};