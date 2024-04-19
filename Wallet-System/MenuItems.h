
#pragma once
#ifndef MENUITEMS_H
#define MENUITEMS_H
#include "CLI.h"
#include "User.h"

class MenuItem {
public:


    static MenuItem* currentMenuItem;

    static User* user;

    MenuItem* Back;

    string name;
    vector<MenuItem*> subMenus;

    MenuItem(string name, MenuItem* Back);

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
    MainMenu(string name, MenuItem* Back);

    bool back() override;

};

class LoginUserMenu : public MenuItem
{
public:
    LoginUserMenu(string name, MenuItem* Back);

    bool update() override;

};

class RegisterUserMenu : public MenuItem
{
public:
    RegisterUserMenu(string name, MenuItem* Back);

    bool update() override;

};

class UserProfileMenu : public MenuItem
{
public:


    UserProfileMenu(string name, MenuItem* Back);

    bool back() override;
    bool update() override;

};


#endif

