
#include <iostream>

#include "Admin.h"
#include "User.h"
#include "Files.h"
#include "CLI.h"
#include "Container.h"

int main() {
	Files::readUsersData();
    Files::readTransactionsData();
	CLI::start();
	Files::writeUsersData(Container::Users);
    Files::writeTransactionsData(Container::allTransactions);
}