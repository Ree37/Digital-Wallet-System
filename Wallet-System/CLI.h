#pragma once

#include "MenuItems.h"
#include "string"

class CLI {
public:
    static void start();
    static void initMenu();
    static void navMenu();
    static void clearCli();
    static void drawCli(bool isValid);
    static std::string invalidMessage(int size);
    static void drawInvalid();
    static int getInput(bool overwrite = false, int size = 0);
};
