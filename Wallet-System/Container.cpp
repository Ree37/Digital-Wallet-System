#include "Container.h"
#include "Files.h"
vector<Transaction> Container:: Transactions;
vector<User*> Container::users; // why This variable here and why pointer ?

void Container::addUser(User* user) {
	Container::users.push_back(user);
}
void Container::addTransaction(User sender, User recipient, float amount)
{
	Transaction newTransaction(sender,recipient,amount);
	Transactions.push_back(newTransaction);
}

