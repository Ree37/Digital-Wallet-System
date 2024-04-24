#include <iostream>

#include "Admin.h"
#include "User.h"
#include "Files.h"
#include "CLI.h"
#include "Container.h"

int main() {
	Files::readDataFromCsvFile();
	CLI::start();
	Files::writeUsersDataFromContainerToFiles(Container::Users);
}