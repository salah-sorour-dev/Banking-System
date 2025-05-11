

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

const string ClientsFileName = "C:\\Users\\SALA77\\OneDrive\\Bureau\\Projects-Github\\Problem-solving-with-C++\\level 4\\Project - Bank 3\\Project - Bank 3\\Clients.txt";
const string UsersFileName = "C:\\Users\\SALA77\\OneDrive\\Bureau\\Projects-Github\\Problem-solving-with-C++\\level 4\\Project - Bank 3\\Project - Bank 3\\Users.txt";

void ShowMainMenue();
void ShowTransactionsScreen();
void ShowLoginScreen();
void ShowManageUsersScreen();

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

struct sUser
{
	string UserName;
	string Password;
	short Permissions;
	bool MarkForDelete = false;
};

enum enPrmissions
{
	eAll = -1,
	elistClients = 1, eAddClient = 2,
	edeleteClient = 4, eupdateClient = 8,
	efindCLient = 16, etransactions = 32,
	emanageUsers = 64
};

sUser CurrentUser;

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;

	short pos = 0;
	string sWord;

	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

sClient ConvertLineToRecord(string Line, string Separator = "#//#")
{
	sClient Client;
	vector<string> vClientData;

	vClientData = SplitString(Line, Separator);

	if (vClientData.size() >= 5) {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]);
    }

	return Client;
}

string ConvertRecordToLine(sClient Client, string Separator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Separator;
	stClientRecord += Client.PinCode + Separator;
	stClientRecord += Client.Name + Separator;
	stClientRecord += Client.Phone + Separator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

sUser ConvertUserLineToRecord(string Line, string Separator = "#//#")
{
	sUser User;
	vector<string> vUsers;

	vUsers = SplitString(Line, Separator);

	User.UserName = vUsers[0];
	User.Password = vUsers[1];
	User.Permissions = stoi(vUsers[2]);

	return User;
}

string ConvertUserRecordToLine(sUser User, string Separator = "#//#")
{
	string stUserRecord = "";

	stUserRecord = User.UserName + Separator;
	stUserRecord += User.Password + Separator;
	stUserRecord += to_string(User.Permissions);

	return stUserRecord;
}

bool ClientExistByAccountNumber(string AccountNumber, string FileName)
{
	vector<sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}

vector<sClient> LoadDataFromFileToVector(string FileName)
{
	vector<sClient> vClient;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClient.push_back(Client);
		}
		MyFile.close();
	}
	return vClient;
}


void PrintClientRecordsLine(sClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(20) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}



//Check access Permissions
bool CheckAccessPermission(enPrmissions Permission)
{
	if (CurrentUser.Permissions == enPrmissions::eAll)
	{
		return true;
	}
	if ((CurrentUser.Permissions & Permission) == Permission)
	{
		return true;
	}
	
	return false;
}


void ShowAllClientScreen()
{

	if (!CheckAccessPermission((enPrmissions)CurrentUser.Permissions))
	{
		return;
	}

	vector<sClient> vClients = LoadDataFromFileToVector(ClientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s)\n";
	cout << "\n___________________________________________";
	cout << "___________________________________________\n\n";

	cout << "| " << left << setw(15) << "AccountNumber";
	cout << "| " << left << setw(10) << "PinCode";
	cout << "| " << left << setw(20) << "Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "AccountBalance";

	cout << "\n___________________________________________";
	cout << "___________________________________________\n\n";

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Client Available In the System!";
	}
	else
	{
		for (sClient Client : vClients)
		{
			PrintClientRecordsLine(Client);
			cout << endl;
		}
	}

	cout << "\n___________________________________________";
	cout << "___________________________________________\n\n";
}


sClient ReadNewClient()
{
	sClient Client;

	cout << "Enter AccountNumber? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, enter another AccountNumber?";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;


	return Client;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;

		MyFile.close();
	}
}

void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
	char AddMore = 'Y';
	do
	{
		//system("cls");
		cout << "Adding New Client:\n\n";

		AddNewClient();
		cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientsScreen()
{

	if (!CheckAccessPermission((enPrmissions)CurrentUser.Permissions))
	{
		return;
	}

	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n-----------------------------------\n";
	AddNewClients();
}


bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{

	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

void PrintClientCard(sClient Client)
{
	cout << "\nThe following are client details:";
	cout << "\n---------------------------------";
	cout << "\nAccount Number : " << Client.AccountNumber;
	cout << "\nPinCode : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance : " << Client.AccountBalance;
	cout << "\n---------------------------------\n";
}


bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector<sClient> SaveClientsDataToFile(string FileName, vector<sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;

	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				//here we only write record are not mark fo delete
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}
	return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure you want delete this client? y/n?";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);

			//Refresh Clients
			vClients = LoadDataFromFileToVector(ClientsFileName);

			cout << "\nClient Deleted Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient with account Number [" << AccountNumber << "] Not Found!\n";
	}
}


string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

void ShowDeleteClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------------\n";

	vector<sClient> vClients = LoadDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}


sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;
	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure you want to update this client? y/n? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientsFileName, vClients);

			cout << "\n\nClient Updated Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient with account Number [" << AccountNumber << "] Not Found!\n";
		return false;
	}
}


void ShowUpdateClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n-----------------------------------\n";

	vector<sClient> vClients = LoadDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}


void ShowFindClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";

	vector<sClient> vClients = LoadDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	sClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient with AccountNumber [" << AccountNumber << "] is not found!";
}


enum enTransactions
{
	eDeposition = 1,
	eTotalBalances = 2,
	eWithdraw = 3,
	eMainMenue = 4
};

bool DepositionAmountByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClients)
{
	char Answer = 'n';
	cout << "\n\nAre you sure yo want perform this transaction? y/n? ";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfuly. New Balance is: " << C.AccountBalance;

				return true;
			}
		}
		return false;
	}



}

void ShowDepositionScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDeposition Screen";
	cout << "\n-----------------------------------\n";

	sClient Client;

	vector<sClient> vClients = LoadDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";

		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);

	double Amount = 0;
	cout << "Please enter deposit amount? ";
	cin >> Amount;

	DepositionAmountByAccountNumber(AccountNumber, Amount, vClients);
}


void PrintAccNumNameAccBalanceRecordLine(sClient Client)
{
	cout << "| " << left << setw(20) << Client.AccountNumber;
	cout << "| " << left << setw(25) << Client.Name;
	cout << "| " << left << setw(17) << Client.AccountBalance;
}


void ShowTotalBalancesScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tTotal Balances Screen";
	cout << "\n-----------------------------------\n";

	vector<sClient> vClients = LoadDataFromFileToVector(ClientsFileName);

	cout << "\n\nClients List (" << vClients.size() << ") Client(s)";
	cout << "\n___________________________________________";
	cout << "_____________________________\n\n";
	cout << "| " << left << setw(20) << "AccountNumber";
	cout << "| " << left << setw(25) << "Name";
	cout << "| " << left << setw(17) << "AccountBalance";
	cout << "\n___________________________________________";
	cout << "_____________________________\n\n";

	double TotalBalances = 0;

	if (vClients.size() == 0)
	{
		cout << "\tNo Client Avialable in the system!";
	}
	else
	{
		for (sClient Client : vClients)
		{
			PrintAccNumNameAccBalanceRecordLine(Client);
			TotalBalances += Client.AccountBalance;

			cout << endl;
		}

	}

	cout << "\n___________________________________________";
	cout << "_____________________________\n\n";
	cout << "\t\t\tTotal Balances: " << TotalBalances;
	cout << "\n___________________________________________";
	cout << "_____________________________\n\n";
}


void ShowWithdrawScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n-----------------------------------\n";

	sClient Client;

	vector<sClient> vClients = LoadDataFromFileToVector(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";

		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);

	double Amount = 0;
	cout << "Please enter withdraw amount? ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance. you can withdraw up to: " << Client.AccountBalance;
		cout << "\nPlease Enter Another Amount? ";
		cin >> Amount;
	}

	DepositionAmountByAccountNumber(AccountNumber, Amount * -1, vClients);
}


void GoBackToTransactionsScreen()
{
	cout << "\nPress any key to go back to Transactions Screen...";
	system("pause>0");
	ShowTransactionsScreen();
}

short ReadTransactionOption()
{
	short Choice = 0;
	cout << "\nChoice what do you want to do? [1 to 4]? ";
	cin >> Choice;
	return Choice;
}

void PerformTransactionsOptions(enTransactions Transaction)
{
	switch (Transaction)
	{
	case eDeposition:
		system("cls");
		ShowDepositionScreen();
		GoBackToTransactionsScreen();
		break;
	case eTotalBalances:
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionsScreen();
		break;
	case eWithdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionsScreen();
		break;
	case eMainMenue:
		ShowMainMenue();
		break;
	default:
		break;
	}
}

void ShowTransactionsScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tTransactions Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposition.\n";
	cout << "\t[2] Total Balances.\n";
	cout << "\t[3] Withdraw.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n";
	PerformTransactionsOptions((enTransactions)ReadTransactionOption());
}


bool UserExistByNameAndPassword(string Name, string FileName, string Password = "")
{
	vector<sUser> vUsers;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			if (Password.empty())
			{
				if (User.UserName == Name)
				{
					MyFile.close();
					return true;
				}
			}
			else
			{
				if (User.UserName == Name && User.Password == Password)
				{
					MyFile.close();
					return true;
				}
			}
			vUsers.push_back(User);
		}
		MyFile.close();
	}
	return false;
}


sUser ReadUser()
{
	sUser User;
	cout << "\nEnter username? ";
	getline(cin >> ws, User.UserName);

	cout << "\nEnter password? ";
	getline(cin, User.Password);

	while (!UserExistByNameAndPassword(User.UserName, UsersFileName, User.Password)) // ana wa7el hna lvalue dyal lpermissions katbeldel sabab ach ma3rafnach
	{
		system("cls");

		cout << "===========================================\n";
		cout << "\t\tLogin Screen\n";
		cout << "===========================================\n";

		cout << "\nInvalid username/password!\n";
		cout << "\nEnter username? ";
		getline(cin, User.UserName);

		cout << "\nEnter password? ";
		getline(cin, User.Password);
	}
	return User;
}

void ShowLoginScreen()
{
	cout << "===========================================\n";
	cout << "\t\tLogin Screen\n";
	cout << "===========================================\n";

	CurrentUser = ReadUser();

}


enum enManageUsersMenue
{
	eListUsers = 1, eAddNewUser = 2,
	eDeleteUser = 3, eUpdateUser = 4,
	eFindUser = 5, eMainMenueOptions = 6
};

vector<sUser> LoadUsersDateFromFileToVector(string FileName)
{
	vector<sUser> vUsers;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			vUsers.push_back(User);
		}
		MyFile.close();
	}
	return vUsers;
}

void PrintUsersRecordsLine(sUser User)
{
	cout << "| " << left << setw(20) << User.UserName;
	cout << "| " << left << setw(20) << User.Password;
	cout << "| " << left << setw(20) << User.Permissions;
}

void ShowAllUsersScreen()
{

	vector<sUser> vUsers = LoadUsersDateFromFileToVector(UsersFileName);

	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s)\n";
	cout << "\n___________________________________________";
	cout << "___________________________________________\n\n";

	cout << "| " << left << setw(20) << "User name";
	cout << "| " << left << setw(20) << "Password";
	cout << "| " << left << setw(20) << "Permissions";;

	cout << "\n___________________________________________";
	cout << "___________________________________________\n\n";

	if (vUsers.size() == 0)
	{
		cout << "\t\t\t\tNo User Available In the System!";
	}
	else
	{
		for (sUser User : vUsers)
		{
			PrintUsersRecordsLine(User);
			cout << endl;
		}
	}

	cout << "\n___________________________________________";
	cout << "___________________________________________\n\n";
}

//Add User


int ReadPermissionToSet()
{
	int permission = 0;
	char Answer = 'n';

	cout << "\nDo you want to give full access? y/n ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		return -1;
	}

	if(Answer == 'n' || Answer == 'N')
	{
		cout << "\n\nDo you want give access to:\n";
		cout << "\nShow clients list? y/n ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
			permission += enPrmissions::elistClients;

		cout << "\nAdd new client? y/n ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
			permission += enPrmissions::eAddClient;

		cout << "\nDelete client? y/n ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
			permission += enPrmissions::edeleteClient;

		cout << "\nUpdate client? y/n ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
			permission += enPrmissions::eupdateClient;

		cout << "\nFind client? y/n ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
			permission += enPrmissions::efindCLient;

		cout << "\nTransactions? y/n ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
			permission += enPrmissions::etransactions;

		cout << "\nManage users? y/n ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
			permission += enPrmissions::emanageUsers;
	}

	return permission;

}
string ReadUserName()
{
	string Username = "";
	cout << "\nPlease Enter Username? ";
	cin >> Username;
	return Username;
}




sUser ReadNewUser()
{
	sUser User;
	cout << "\nEnter username? ";
	getline(cin >> ws, User.UserName);


	while (UserExistByNameAndPassword(User.UserName, UsersFileName))
	{
		cout << "\nUser with [" << User.UserName << "] already exists, enter another Username?";
		getline(cin, User.UserName);
	}
	
	
	cout << "Enter password? ";
	getline(cin, User.Password);

	User.Permissions = ReadPermissionToSet();

	return User;
}

void AddNewUser()
{
	sUser User;
	User = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
	char AddMore = 'Y';
	do
	{
		//system("cls");
		cout << "Adding New User:\n\n";

		AddNewUser();

		cout << "\nUser Added Successfully, do you want to add more Users ? Y / N ? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}




bool FindUserByUsername(string Username, vector<sUser> vUsers, sUser& User)
{
	for (sUser U : vUsers)
	{
		if (U.UserName == Username)
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool FindUserByUsernameAndPassword(string Username, string Password, sUser& User)
{
	vector<sUser> vUsers = LoadUsersDateFromFileToVector(UsersFileName);

	for (sUser U : vUsers)
	{
		if (U.UserName == Username && U.Password == Password)
		{
			User = U;
			return true;
		};
	}
	return false;
}

void PrintUserCard(sUser User)
{
	cout << "\nThe following are User details:";
	cout << "\n---------------------------------";
	cout << "\nUsername : " << User.UserName;
	cout << "\nPassword : " << User.Password;
	cout << "\nPermissions : " << User.Permissions;
	cout << "\n---------------------------------\n";
}



bool MarkUserForDeleteByUsername(string Username, vector<sUser>& vUsers)
{
	for (sUser& U : vUsers)
	{
		if (U.UserName == Username)
		{
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector<sUser> SaveUsersDataToFile(string FileName, vector<sUser> vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;

	if (MyFile.is_open())
	{
		for (sUser U : vUsers)
		{
			if (U.MarkForDelete == false)
			{
				//here we only write record are not mark fo delete
				DataLine = ConvertUserRecordToLine(U);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}
	return vUsers;
}

bool DeleteUserByUsername(string Username, vector<sUser>& vUsers)
{
	if (Username == "Admin")
	{
		cout << "\n\nYou cannot delete this user!";
		return false;
	}

	sUser User;
	char Answer = 'n';

	if (FindUserByUsername(Username, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\nAre you sure you want delete this User? y/n?";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			MarkUserForDeleteByUsername(Username, vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);

			//Refresh Clients
			vUsers = LoadUsersDateFromFileToVector(UsersFileName);

			cout << "\nUser Deleted Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nUser with Username [" << Username << "] is Not Found!\n";
	}
}



sUser ChangeUserRecord(string Username)
{
	sUser User;

	cout << "\n\nEnter Username? ";
	getline(cin >> ws, User.UserName);

	cout << "Enter Password? ";
	getline(cin, User.Password);

	User.Permissions = ReadPermissionToSet();

	return User;
}

bool UpdateUserByUsername(string Username, vector<sUser>& vUsers)
{
	sUser User;
	char Answer = 'n';

	if (FindUserByUsername(Username, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\nAre you sure you want to update this User? y/n? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			for (sUser& U : vUsers)
			{
				if (U.UserName == Username)
				{
					U = ChangeUserRecord(Username);
					break;
				}
			}
			SaveUsersDataToFile(UsersFileName, vUsers);

			cout << "\n\nUser Updated Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nUser with Username [" << Username << "] is Not Found!\n";
		return false;
	}
}





//Add User
void ShowAddNewUsersScreen()
{

	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Users Screen";
	cout << "\n-----------------------------------\n";
	AddNewUsers();
}

//Find User
void ShowFindUserScreen()
{

	cout << "\n-----------------------------------\n";
	cout << "\tFind User Screen";
	cout << "\n-----------------------------------\n";

	sUser User;
	vector<sUser> vUsers = LoadUsersDateFromFileToVector(UsersFileName);
	string Username = ReadUserName();
	if (FindUserByUsername(Username, vUsers, User))
		PrintUserCard(User);
	else
		cout << "\nUser with Username [" << Username << "] is Not Found!\n";
}

//Delete User
void ShowDeleteUserScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDelete User Screen";
	cout << "\n-----------------------------------\n";
	
	vector<sUser> vUsers = LoadUsersDateFromFileToVector(UsersFileName);
	string Username = ReadUserName();
	DeleteUserByUsername(Username, vUsers);
}

//Update User
void ShowUpdateUserScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate User Info Screen";
	cout << "\n-----------------------------------\n";

	vector<sUser> vUsers = LoadUsersDateFromFileToVector(UsersFileName);
	string Username = ReadUserName();
	UpdateUserByUsername(Username, vUsers);
}





void GoBackToManageUsersMenueScreen()
{
	cout << "\nPress any key to go back to Manage Users Menue Screen...";
	system("pause>0");
	ShowManageUsersScreen();
}

short ReadManageUsersMenueOption()
{
	cout << "Choose what do you want to do? [1 to 6]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void PerformManageUsersMenueOption(enManageUsersMenue ManageUsersOption)
{
	switch (ManageUsersOption)
	{
	case eListUsers:
		system("cls");
		ShowAllUsersScreen();
		GoBackToManageUsersMenueScreen();
		break;
	case eAddNewUser:
		system("cls");
		ShowAddNewUsersScreen();
		GoBackToManageUsersMenueScreen();
		break;
	case eDeleteUser:
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUsersMenueScreen();
		break;
	case eUpdateUser:
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUsersMenueScreen();
		break;
	case eFindUser:
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUsersMenueScreen();
		break;
	case eMainMenueOptions:
		ShowMainMenue();
		break;
	default:
		break;
	}
}

void ShowManageUsersScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tManage Users Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "===========================================\n";
	PerformManageUsersMenueOption((enManageUsersMenue)ReadManageUsersMenueOption());
}


enum enMainMenueOption
{
	eListClients = 1, eAddNewClient = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eTransactions = 6,
	eManageUsers = 7, eLogout = 8
};

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}

short ReadMainMenueOption()
{
	cout << "Choose what do you want to do? [1 to 8]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void PerformMainMenueOption(enMainMenueOption MainMenueOption)
{
	switch (MainMenueOption)
	{
	case eListClients:
		system("cls");
		ShowAllClientScreen();
		GoBackToMainMenue();
		break;
	case eAddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;
	case eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	case eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	case eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	case eTransactions:
		system("cls");
		ShowTransactionsScreen();
		GoBackToMainMenue();
		break;
	case eManageUsers:
		system("cls");
		ShowManageUsersScreen();
		break;
	case eLogout:
		system("cls");
		ShowLoginScreen();
		ShowMainMenue();
		break;
	default:
		break;
	}
}

void ShowMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===========================================\n";
	PerformMainMenueOption((enMainMenueOption)ReadMainMenueOption());
}





int main()
{
	ShowLoginScreen();
	ShowMainMenue();

	system("pause>0");
	return 0;
}

