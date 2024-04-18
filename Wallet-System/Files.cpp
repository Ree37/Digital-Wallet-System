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
void Files::writeUsersData(User* inputUser)
{
	ofstream usersFile;
	usersFile.open(userData, ios::app); // Open the file in append mode
	if (usersFile.is_open())
	{
		// Input Sanitization: wrap in quotes to prevent comma insertion which breaks csv
		// TODO: Sanitize Excel formula syntax to prevent csv injection
		usersFile << "\"" << inputUser->getUsername() << "\",";
        usersFile << "\"" << inputUser->getPassword() << "\",";
        usersFile << "\"" << inputUser->getBalance() << "\"" << endl;
	}
	usersFile.close();
}
void Files::writeTransactionsData(string sender, string recipient, float amount)
{
	ofstream Transactionsfile;
	Transactionsfile.open(transactionsData, ios::app); // Open the file in append mode
	if (Transactionsfile.is_open())
	{
		// Input Sanitization: wrap in quotes to prevent comma insertion which breaks csv
		// TODO: Sanitize Excel formula syntax to prevent csv injection
		Transactionsfile << "\"" << sender << "\",";
		Transactionsfile << "\"" << recipient << "\",";
		Transactionsfile << "\"" << amount << "\"," << endl;
	}
}
void Files::updateBalance(User sender, User recipient, float amount)
{
	ifstream usersfile;
	usersfile.open(userData);
	string records;
	while (getline(usersfile,records))
	{
		stringstream record(records);
		string userName;
		string loopOnCell;
		float finalBalanceForsender;
		float finalBalanceForRecipient;
		if (getline(record,userName,','))
		{
			if (userName == sender.getUsername())
			{
				getline(record,loopOnCell, ',');
				getline(record, loopOnCell, ',');
				finalBalanceForsender = stof(loopOnCell) - amount;// unfinished
			}
			if (userName == recipient.getUsername())
			{
				getline(record, loopOnCell, ',');
				getline(record, loopOnCell, ',');
				finalBalanceForRecipient = stof(loopOnCell) + amount;// unfinished
			}
		}
		
	}

	
}
User* Files::readData(string userName)
{	
	string password;
	float balance;
	ifstream data;
	ifstream transactions; 
	while (data.peek()!=EOF)
	{
		string records;
		while (getline(data,records))
		{
			stringstream record(records);
			string cell;
			// read username
			if (getline(record , cell , ','))
			{
				if (userName == cell)
				{
					if (getline(record,cell,','))
					{
						password = cell;
					}
					if (getline(record, cell, ','))
					{
						balance = stof (cell);
					}
				}
			}
			
						
		}
		
	}
	//// Constructor with balance 
	User*  userDataFromCsv = new User(userName, password);
	return userDataFromCsv;
}