#include "MenuItems.h"
#include "User.h"
#include "Security/bcrypt/BCryptDLL.h"
#include "Files.h"
#include "Security/Utils.h"



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
    cout << "Input 'x' to leave login menu\n\n";
    while (true) {

        string username, password;


        cout << "Enter Username: ";
        cin >> username;

        if (exitCommand(username))
            return true;


        cout << "Enter Password: ";
        cin >> password;

        if (exitCommand(password))
            return true;


            Files files;
            User* userData = files.readData(username);
            try {
                if (!userData) {
                    throw exception();
                }
                if (!BCryptDLL::validatePassword(password, userData->getPassword())) {
                    throw exception();
                }
            }
            catch (exception e) {
                CLI::clearCli();
                cout << "Input 'x' to leave login menu\n\n";
                cout << "Wrong Username or Password\n\n";
                continue;
            }
           
            MenuItem::user = userData;
            currentMenuItem = currentMenuItem->getSubMenus()[0];

            break;
    }

};

RegisterUserMenu::RegisterUserMenu(string name, MenuItem* Back) : MenuItem(name, Back) {};

bool RegisterUserMenu::update() {
    CLI::clearCli();

    string username, password;
    cout << "Input 'x' to leave register menu\n";

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

            if (!Utils::checkPasswordPolicy(password))
            {
                throw std::invalid_argument("Password is weak. It should include an uppercase letter, a lowercase letter, a number, a special character, and be at least 8 characters long.");
            }
            string hash = BCryptDLL::generateHash(password, 12);

            MenuItem::user = new User(username, hash);

            Files files;
            files.writeUsersData(*MenuItem::user);

            currentMenuItem = currentMenuItem->getSubMenus()[0];

            break;
        }
        catch (exception e) {
            CLI::clearCli();
            cout << "Input 'x' to leave register menu\n\n";

            cout << e.what() << '\n';
            cout << "Enter Username: " << username << "\n";

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

