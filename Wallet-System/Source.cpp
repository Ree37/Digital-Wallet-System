
#include <iostream>

#include "Admin.h"
#include "User.h"
#include "Files.h"
#include "CLI.h"
#include "Container.h"

int main() {
	MenuItem::admin->setName("admin");
	MenuItem::admin->setPassword("$2a$12$RQ8XZhi9Tod7M2FvDgzBeeXfyKfHovi.aJDXCpstZUlY3RoVYYwFO");

	Files::readUsersData();
    Files::readTransactionsData();
	CLI::start();
	Files::writeUsersData(Container::Users);
    Files::writeTransactionsData(Container::allTransactions);
}