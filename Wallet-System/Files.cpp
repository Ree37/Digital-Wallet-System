#include "Files.h"
#include "User.h"
#include <sstream>
#include "Security/Utils.h"
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
		usersFile << "Balance" << ",";
		usersFile << "isHas2FA" << ",";
		usersFile << "TOTP_Secret" << endl;
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
<<<<<<< HEAD
		for (User* user : users)
		{	
		usersFile << "\"" << user->getUsername() << "\",";
        usersFile << "\"" << user->getPassword() << "\",";
        usersFile << "\"" << user->getBalance() << "\"" << endl;	
		}
			
=======
		usersFile << "\"" << inputUser.getUsername() << "\",";
        usersFile << "\"" << inputUser.getPassword() << "\",";
        usersFile << "\"" << inputUser.getBalance() << "\",";
		usersFile << "\"" << inputUser.getIsHas2FA() << "\",";
		usersFile << "\"" << inputUser.getTotpSecret() << "\"" << endl;
>>>>>>> a804d5663436088e89f295512b47cd86722dd1e8
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
<<<<<<< HEAD
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
=======
	while (std::getline(usersfile,records))
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

void Files::updateisHas2FA(User user)
{
	ifstream usersfile;
	usersfile.open(userData);
	string records;
	string tempFileName = "temp.csv"; // Temporary file to store modified content
	ofstream tempfile(tempFileName);

	while (std::getline(usersfile, records))
	{
		stringstream record(records);
		string userName;
		string cell;
		string result = "";

		if (getline(record, userName, ','))
		{
			if (userName == user.getUsername())
			{
				result += '\"' + userName + '\"' + ',';
				getline(record, cell, ',');
				result += '\"' + cell + '\"' + ',';
				getline(record, cell, ',');
				result += '\"' + cell + '\"' + ',';
				getline(record, cell, ',');
				result += '\"' + std::to_string(user.getIsHas2FA() ? 1 : 0) + '\"' + ',';
				getline(record, cell, ',');
				result += '\"' + cell + '\"';
				tempfile << result << endl; // Write modified line to temporary file
			}
			else
			{
				tempfile << records << endl; // Write unchanged line to temporary file
			}
		}
	}

	usersfile.close();
	tempfile.close();

	// Replace the original file with the temporary file
	remove(userData.c_str());          // Remove the original file
	rename(tempFileName.c_str(), userData.c_str()); // Rename temporary file to original file
}

void Files::updateTotpSecret(User user)
{
	ifstream usersfile;
	usersfile.open(userData);
	string records;
	string tempFileName = "temp.csv"; // Temporary file to store modified content
	ofstream tempfile(tempFileName);

	while (std::getline(usersfile, records))
	{
		stringstream record(records);
		string userName;
		string cell;
		string result = "";

		if (getline(record, userName, ','))
		{
			if (userName == user.getUsername())
			{
				result += '\"' + userName + '\"' + ',';
				getline(record, cell, ',');
				result += '\"' + cell + '\"' + ',';
				getline(record, cell, ',');
				result += '\"' + cell + '\"' + ',';
				getline(record, cell, ',');
				result += '\"' + cell + '\"' + ',';
				getline(record, cell, ',');
				result += '\"' + user.getTotpSecret() + '\"';
				tempfile << result << endl; // Write modified line to temporary file
			}
			else
			{
				tempfile << records << endl; // Write unchanged line to temporary file
			}
		}
	}

	usersfile.close();
	tempfile.close();

	// Replace the original file with the temporary file
	remove(userData.c_str());          // Remove the original file
	rename(tempFileName.c_str(), userData.c_str()); // Rename temporary file to original file
}

void Files::update2FA(User* user, bool isHas2FA, std::string secret)
{
	user->setIsHas2FA(true);
	user->setTotpSecret(secret);
	updateisHas2FA(*user);
	updateTotpSecret(*user);
}

User* Files::readData(string userName)
{	
	string password;
	float balance;
	bool isHas2FA;
	string totpSecret;
	//Utils::encryptFiles(userData);
	ifstream data;
	data.open(userData);
	ifstream transactions; 
	while (data.peek()!=EOF)
	{
		string records;
		while (std::getline(data,records))
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
						// until know more about user
						balance = stof (cell);
					}
					if (getline(record, cell, ','))
					{
						bool b;
						istringstream(cell) >> b;
						isHas2FA = b;
					}
					if (getline(record, cell, ','))
					{
						totpSecret = cell;
					}
					User* user = new User(userName, password);

					user->setIsHas2FA(isHas2FA);
					user->setTotpSecret(totpSecret);

					return user;
				}
			}
			
						
		}
		
	}
	return NULL;
}

bool Files::getline(std::istream& is, std::string& str, char delim) {
	if (std::getline(is, str, delim)) {
		str = str.substr(1, str.length() - 2); // Strip Quotes
		return true;
	}
	return false;
}
>>>>>>> a804d5663436088e89f295512b47cd86722dd1e8
