#include "Files.h"
#include <sstream>
string transactionsData = "Transactions.csv";
string userData = "UsersData.csv";
void Files::create()
{
	ofstream usersFile,Transactions;
	usersFile.open(transactionsData);
	usersFile.open(userData);
	if (usersFile.is_open())
	{
		usersFile << "UserName" << ",";
		usersFile << "Password" << ",";
		usersFile << "Balance" << "," << endl;
	}
	if (Transactions.is_open())
	{
		usersFile << "Sender" << ",";
		usersFile << "recipient" << ",";
		usersFile << "amount" << "," << endl;
	}
	usersFile.close();
	Transactions.close();
}
void Files::writeUsersData(User inputUser)
{
	ofstream usersFile;
	usersFile.open(userData);
	if (usersFile.is_open())
	{
		usersFile << inputUser.getUsername() << ",";
		usersFile << inputUser.getPassword() << ",";
		usersFile << inputUser.getBalance() << "," << endl;
	}
	usersFile.close();
}
void Files::writeTransactionsData(User sender, User recipient, float amount)
{
	ofstream Transactionsfile;
	Transactionsfile.open(transactionsData);
	if (Transactionsfile.is_open())
	{
		Transactionsfile << sender.getUsername() << ","; 
		Transactionsfile << recipient.getUsername() << ",";
		Transactionsfile << amount << "," << endl;
	}
	updateBalance(sender, recipient, amount);
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
User Files::readData(string userName)
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
	//User  userDataFromCsv = new User(userName, password);
	//return userDataFromCsv;
}