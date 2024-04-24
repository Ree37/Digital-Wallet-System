#include <iostream>
#include "Admin.h"
#include "User.h"
#include "Files.h"
#include "CLI.h"
using namespace std;
int main() {
	//Admin admin("Admin1", "Admin1");
  Files::readDataFromCsvFile();
	CLI::start();
  Files::writeUsersDataFromContainerToFiles(Container::Users);
}