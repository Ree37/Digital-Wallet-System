#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class CLI {

public:
    static void start();
    static void initMenu();
    static void navMenu();
    static void clearCli();
    static void drawCli(bool isValid);
    static int getInput();
};

/*

class MenuItem {
public:


    static MenuItem* currentMenuItem;

    MenuItem* Back;

    string name;
    vector<MenuItem> subMenus;

    MenuItem(string name, MenuItem* Back);

    static void printMenu(MenuItem* menuItem);

    string getName();

    void setName(string name);

    vector<MenuItem> getSubMenus();

    bool hasSubMenus();

    bool hasBack();

    void addSubMenu(MenuItem subMenu);

    bool update();

    virtual bool back();
};
*/


#endif