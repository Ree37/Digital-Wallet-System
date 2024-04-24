#pragma once

class CLI {
public:
    static void start();
    static void initMenu();
    static void navMenu();
    static void clearCli();
    static void drawCli(bool isValid);
    static int getInput();
};
