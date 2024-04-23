#include "Files.h"
#include "User.h"
#include <sstream>
string transactionsData = "Transactions.csv";
string userData = "UsersData.csv";
void Files::create()
{
	// check if file already exists else create it
	ofstream usersFile,Transactions;
	Transactions.open(transactionsData);
	usersFile.open(userData);
	if (usersFile.is_open())
	{
		usersFile << "UserName" << ",";
		usersFile << "Password" << ",";
		usersFile << "Balance" << endl;
	}
	if (Transactions.is_open())
	{
		Transactions << "Sender" << ",";
		Transactions << "recipient" << ",";
		Transactions << "amount" << endl;
	}
	usersFile.close();
	Transactions.close();
}
void Files::writeUsersDataFromContainerToFiles(vector <User*> users)
{
	ofstream usersFile;
	usersFile.open(userData, ios::app); // Open the file in append mode
	if (usersFile.is_open())
	{
		// Input Sanitization: wrap in quotes to prevent comma insertion which breaks csv
		// TODO: Sanitize Excel formula syntax to prevent csv injection
		for (User* user : users)
		{	
		usersFile << "\"" << user->getUsername() << "\",";
        usersFile << "\"" << user->getPassword() << "\",";
        usersFile << "\"" << user->getBalance() << "\"" << endl;	
		}
			
	}
	usersFile.close();
}
void Files::writeTransactionsDataFromContainerToFiles(vector<Transaction>Transactions)
{
	ofstream Transactionsfile;
	Transactionsfile.open(transactionsData, ios::app); // Open the file in append mode
	if (Transactionsfile.is_open())
	{		
		for (Transaction trans : Transactions)
		{
		/*	TransactionsFile << "\"" << trans.<< "\",";
			usersFile << "\"" << trans->get() << "\","; 
			usersFile << "\"" << trans->get() << "\"" << endl;*/ // Make getters for transaction class (Remo)
		}

	}
}
stringstream getLineFromData(ifstream& data)
{
	string records;
	getline(data, records);
	stringstream record(records);
	return record;

}
string getCellFromLine(stringstream& record)
{
	string cell;
	getline(record, cell, ',');
	size_t start = cell.find_first_not_of(" \t");
	size_t end = cell.find_last_not_of(" \t");
	if (start == string::npos || end == string::npos)
	{
		cell = ""; // Empty string if no non-whitespace characters found
	}
	else
	{
		cell = cell.substr(start, end - start + 1);
	}
	return cell;
}
User* readDataFromCsvFile(string userName)
{
	ifstream names;
	names.open("Names.csv");
	while (names.peek() != EOF)
	{
		stringstream Line = getLineFromData(names);
		string cell = getCellFromLine(Line);

		if (userName == cell)
		{
			string storedPassword = getCellFromLine(Line);
			float storedBalance = stof(getCellFromLine(Line));

			User* user = new User(cell, storedPassword);
			user->setBalance(storedBalance);
			return user;
		}
	}
	names.close();
	return nullptr;
}
