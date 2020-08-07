#include "Account.h"

Account Account::loadAnAccount(ifstream& fin)
{
	fin.ignore();
	getline(fin, username);
	getline(fin, passHash);
	fin >> ID;
	return *this;
}

string Account::inputPassword()
{
	string passWord = "";
	char c = _getch();
	while (c != '\n' && c != '\r')
	{
		if (c == '\b')
		{
			int count = passWord.length();
			if (count > 0)
			{
				std::cout << "\b \b";
				passWord = passWord.substr(0, passWord.length() - 1);
			}
		}
		else
			if (c == 27)
			{
				return "";
			}
			else
			{
				std::cout << "*";
				passWord += c;
			}
		c = _getch();
	}
	std::cout << "\n";
	return passWord;
}

string Account::HashPassword(string passWord)
{
	SHA256 sha256;
	string hashPass = sha256(passWord);
	return hashPass;
}

void Account::saveAccount(ofstream& fout)
{
	fout << username << endl;
	fout << passHash << endl;
	fout << ID << endl << endl;
}

void Account::loadListIDOfUser()
{
	ifstream fin("Account/UserID.txt");
	if (!fin.is_open())
	{
		cout << "Can't load list ID of user!!" << endl;
		return;
	}
	int nID = 0;
	string ID;
	fin >> nID;
	fin.ignore();
	for(int i=0;i<nID;i++) {
		getline(fin, ID);
		listID.push_back(ID);
	}

}

string Account::createNewID()
{
 string newID;
	loadListIDOfUser();
	newID = to_string(stoi(listID[listID.size() - 1]) + 1);
	listID.push_back(newID);
	saveListIDOfUSer();
	listID.clear();
	return newID;
}

void Account::saveListIDOfUSer()
{
	ofstream fout("Account/UserID.txt");
	if (!fout.is_open()) {
		cout << "Can't save list ID of user!!" << endl;
		return;
	}
	fout << listID.size() << endl;
	for (int i = 0; i < listID.size(); i++)
		fout << listID[i];
	fout.close();
}

Account Account::createNewAccount()
{
	string password, username, retypePassword;
	string ID;
		cout << "Username:";
		cin.ignore();
		getline(cin, username);
	do {
		cout << "Password: ";
		password = inputPassword();
		cout << "Retype password: ";
		string retypePassword;
		retypePassword = inputPassword();
		if (password != retypePassword) {
			cout << "Retype password and new password is not match\nPress any key to try again";
			system("cls");
			cout << "Username:" << username << endl;
		}
	} while (password != retypePassword);
	
	this->ID = createNewID();
	this->username = username;
	this->passHash = HashPassword(password);
	return *this;
}

void ListAccount::loadListAccount(string filename)
{
	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "Can't open " << filename << "!!" << endl;
		return;
	}
	int nAccount;
	fin >> nAccount;
	for (int i = 0; i < nAccount; i++) 
		listAccount.push_back(Account::loadAnAccount(fin));
	fin.close();
}

int ListAccount::login(string username, string password)
{
	int result = 0;
	string listFile[2] = { "Admin.txt", "User.txt" };
	//0-Failed	1-User	2-Admin
	password = Account::HashPassword(password);
	if (username.substr(0, 7) == "197.000") {
		loadListAccount(listFile[0]);
		for (int i = 0; i < listAccount.size(); i++)
			if (password == listAccount[i].getPassHass() && username == listAccount[i].getUsername())
				result = 2;
	}
	else {
		loadListAccount(listFile[1]);
		for (int i = 0; i < listAccount.size(); i++)
			if (password == listAccount[i].getPassHass() && username == listAccount[i].getUsername())
				result = 1;
	}
	listAccount.clear();
	return result;
}

void ListAccount::saveListAccount(const int& nAccount, string filename)
{
	ofstream fout(filename);
	if (!fout.is_open()) {
		cout << "Can't save Account!!" << endl;
		return;
	}
	fout << nAccount << endl;
	for (int i = 0; i < listAccount.size(); i++)
		listAccount[i].saveAccount(fout);
	fout.close();
}

void ListAccount::SignUp()
{
	Account newAccount;
	newAccount.createNewAccount();
	loadListAccount("Account/User.txt");
	listAccount.push_back(newAccount);
	saveListAccount(listAccount.size(), "Account/User.txt");
	listAccount.clear();
}

void ListAccount::removeAccount() {
	string removeID = "";
	cout << "Input ID of user to remove: ";
	cin.ignore();
	getline(cin, removeID);
	loadListAccount("Account/User.txt");
	for (int i = 0; i < listAccount.size(); i++)
		if (removeID == listAccount[i].getID())
			listAccount.erase(listAccount.begin() + i);
	saveListAccount(listAccount.size(), "Account/User.txt");
	listAccount.clear();
}


