#include "CLI.h"
#include "MenuItems.h"
#include <iostream>
#include <cstdlib>
using namespace std;

void CLI::start() {
    CLI::initMenu();
    CLI::navMenu();
}





MainMenu mmainMenu("Main Menu", &mmainMenu);


MenuItem* MenuItem::currentMenuItem = &mmainMenu;//new MainMenu("Main Menu", NULL); //= mmainMenu;

    LoginUserMenu loginUser("Login User", MenuItem::currentMenuItem);

        UserProfileMenu userProfile("User Profile", &loginUser);

            MenuItem sendMoney("Send Money", &userProfile);
            MenuItem requestMoney("Request Money", &userProfile);
            MenuItem addMoney("Add Money", &userProfile);
            MenuItem viewRequests("View Requests", &userProfile);
            MenuItem viewTransactions("View Transactions", &userProfile);
            MenuItem settings("Settings", &userProfile);




    LoginUserMenu registerUser("Register User", MenuItem::currentMenuItem);

    MenuItem loginAdmin("Login Admin", MenuItem::currentMenuItem);



void CLI::initMenu() {

    MenuItem::currentMenuItem->addSubMenu(&loginUser);
    MenuItem::currentMenuItem->addSubMenu(&registerUser);
    MenuItem::currentMenuItem->addSubMenu(&loginAdmin);

        loginUser.addSubMenu(&userProfile);
        registerUser.addSubMenu(&userProfile);

            userProfile.addSubMenu(&sendMoney);
            userProfile.addSubMenu(&requestMoney);
            userProfile.addSubMenu(&addMoney);
            userProfile.addSubMenu(&viewRequests);
            userProfile.addSubMenu(&viewTransactions);
            userProfile.addSubMenu(&settings);

}

void CLI::navMenu() {

    while (MenuItem::currentMenuItem->update()) {}
}


void CLI::clearCli() {


    
    try {
        #ifdef _WIN32
        std::system("cls");
        #else
        // Assume Unix-like system
        std::cout << "\033[H\033[2J";
        std::cout.flush();
        #endif
    }
    catch (const std::exception& e) {
        std::cout << "An error occurred while clearing the CLI: " << e.what() << std::endl;
    }
    




}
void CLI::drawCli(bool isValid) {

    if ( !dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem) ) {
        clearCli();
    }
    

    if (!isValid) {
        cout << "\nInvalid choice. Please enter 'x' or a number between 1 and " + MenuItem::currentMenuItem->getSubMenus().size();
    }

    if (!dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem)) {
        cout << "\nCurrent Menu: " + MenuItem::currentMenuItem->getName() << "\n\n";
    }
    MenuItem::printMenu(MenuItem::currentMenuItem);

    string eofTerminal = "\033[9999H";
    cout << eofTerminal;

    cout << "Enter your choice: ";
}
int CLI::getInput() {


    int choice;

    string input;
    cin >> input;

    try {
        choice = stoi(input);
    }
    catch (invalid_argument e) {
        choice = input[0];
    }

    if ((choice < 1 || choice > MenuItem::currentMenuItem->getSubMenus().size()) && choice != 'x') {
        return 0;
    }
    else {
        return choice;
    }
}

/*
MenuItem::MenuItem(std::string name, MenuItem* Back) : name(name), Back(Back) {}

void MenuItem::printMenu(MenuItem* menuItem) {


    for (size_t i = 0; i < menuItem->getSubMenus().size(); i++) {
        
        cout << i+1 << "] " << menuItem->getSubMenus().at(i).name << "\n";
    }

    if (menuItem->Back == menuItem) {
        cout << "x] Exit" << std::endl;
    }
    else if (menuItem->hasBack()) {
        cout << "x] Back" << std::endl;
    }
}

std::string MenuItem::getName() {
    return this->name;
}

void MenuItem::setName(std::string name) {
    this->name = name;
}

std::vector<MenuItem> MenuItem::getSubMenus() {
    return this->subMenus;
}

bool MenuItem::hasSubMenus() {
    return !subMenus.empty();
}

bool MenuItem::hasBack() {
    return Back != nullptr;
}

void MenuItem::addSubMenu(MenuItem subMenu) {
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
        desiredSubmenu = currentMenuItem->getSubMenus()[choice - 1];
        currentMenuItem = &desiredSubmenu;
    }
    return true;
}

bool MenuItem::back() {
    currentMenuItem = Back;
    return true;
}
*/
/*
MainMenu::MainMenu(string name, MenuItem* Back) : MenuItem(name, Back) {};

bool MainMenu::back() {

    CLI::clearCli();

    cout << "Exiting...\n";

    return false;
};*/
