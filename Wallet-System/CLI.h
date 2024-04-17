#include <iostream>
#include <vector>
#include <string>

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


enum EnumMenus { mainMenu };

class MenuItem {
public:


    static MenuItem* currentMenuItem;

    MenuItem* Back;

    string name;
    vector<MenuItem> subMenus;


    EnumMenus enumMenus;

    MenuItem(string name, MenuItem* Back);

    static void printMenu(MenuItem menuItem);

    string getName();

    void setName(string name);

    vector<MenuItem> getSubMenus();

    bool hasSubMenus();

    bool hasBack();

    void addSubMenu(MenuItem subMenu);

    bool update();

    bool back();
};

