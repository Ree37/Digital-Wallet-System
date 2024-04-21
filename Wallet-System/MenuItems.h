
#pragma once
#ifndef MENUITEMS_H
#define MENUITEMS_H
#include "CLI.h"
#include "User.h"
#include <stack>


class MenuItem {
public:



    static User* user;

    static stack<MenuItem*> currentMenuItem;

    string name;
    vector<MenuItem*> subMenus;

    MenuItem(string name);

    static void printMenu(MenuItem* menuItem);

    string getName();

    void setName(string name);

    vector<MenuItem*> getSubMenus();

    bool hasSubMenus();

    bool hasBack();

    void addSubMenu(MenuItem* subMenu);

    virtual bool update();

    virtual bool back();

    bool exitCommand(string s);
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




#endif

