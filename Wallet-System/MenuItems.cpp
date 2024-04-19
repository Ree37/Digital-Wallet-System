#include "MenuItems.h"
#include "User.h"



//MenuItem desiredSubmenu("", NULL);

User* MenuItem::user;

MenuItem::MenuItem(std::string name, MenuItem* Back) : name(name), Back(Back) {
}

void MenuItem::printMenu(MenuItem* menuItem) {


    for (size_t i = 0; i < menuItem->getSubMenus().size(); i++) {

        cout << i + 1 << "] " << menuItem->getSubMenus().at(i)->name << "\n";
    }

    if (dynamic_cast<MainMenu*>(MenuItem::currentMenuItem)) {
        cout << "x] Exit" << "\n";
    }
    else if (dynamic_cast<UserProfileMenu*>(MenuItem::currentMenuItem))
    {
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
    return Back != nullptr;
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
        currentMenuItem = currentMenuItem->getSubMenus()[choice - 1];
    }
    return true;
}

bool MenuItem::back() {
    currentMenuItem = Back;
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

MainMenu::MainMenu(string name, MenuItem* Back) : MenuItem(name, Back) {};

bool MainMenu::back() {

    CLI::clearCli();

    cout << "Exiting...\n";

    return false;
};


LoginUserMenu::LoginUserMenu(string name, MenuItem* Back) : MenuItem(name, Back) {};

bool LoginUserMenu::update() {
    CLI::clearCli();

    string username, password;
    cout << "Input 'x' to leave login menu\n";

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

            currentMenuItem = currentMenuItem->getSubMenus()[0];
            MenuItem::user = new User(username, password);
            break;
        }
        catch (exception e) {
            currentMenuItem = currentMenuItem->Back;

            CLI::clearCli();
            std::cout << e.what() << '\n';
        }
    }


};

UserProfileMenu::UserProfileMenu(string name, MenuItem* Back) : MenuItem(name, Back) {};

bool UserProfileMenu::update() {

    CLI::clearCli();

    cout << "Current User: " << MenuItem::user->getUsername() << "\n";
    cout << "Available Balance: " << MenuItem::user->getBalance() << "\n";

    MenuItem::update();

    return true;
};

bool UserProfileMenu::back() {
    currentMenuItem = Back->Back;
    return true;
};

