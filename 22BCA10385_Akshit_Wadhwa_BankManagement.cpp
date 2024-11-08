#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;

class BankAccount {
protected:
    string name;
    int accountNum;
    double balance;
    string password;
    int transactionCount;
    string accountType;
    vector<string> transactions;
    const double minBalance = 1000;

public:
    BankAccount(string n, int ac, double bal, string pass, string type) 
        : name(n), accountNum(ac), balance(bal), password(pass), accountType(type), transactionCount(0) {
        transactions.push_back("Account created with initial balance: $" + to_string(bal));
    }

    string getName() const { return name; }
    int getAccountNum() const { return accountNum; }
    double getBalance() const { return balance; }
    string getAccountType() const { return accountType; }
    bool verifyPassword(string pass) { return pass == password; }

    void deposit(double amount) {
        balance += amount;
        transactions.push_back("Deposited: $" + to_string(amount));
        cout << "\n\t\tDeposit Successful!";
    }

    void withdraw(double amount) {
        if (balance >= amount) {
            if (accountType == "Current" && transactionCount >= 5) {
                cout << "\n\t\tTransaction limit exceeded for Current Account!";
                return;
            }
            balance -= amount;
            transactionCount++;
            transactions.push_back("Withdrew: $" + to_string(amount));
            cout << "\n\t\tWithdrawal Successful!";
        } else {
            cout << "\n\t\tInsufficient Balance!";
        }
    }

    void addInterest(double rate) {
        if (accountType == "Savings") {
            double interest = balance * (rate / 100);
            balance += interest;
            transactions.push_back("Interest added: $" + to_string(interest));
            cout << "\n\t\tInterest added: $" << fixed << setprecision(2) << interest;
        }
    }

    void applyMonthlyPenalties() {
        if (accountType == "Current" && balance < minBalance) {
            double penalty = 50;
            balance -= penalty;
            transactions.push_back("Low balance penalty: -$" + to_string(penalty));
            cout << "\n\t\tLow balance penalty applied for Account " << accountNum;
        }
    }

    void resetMonthlyTransactions() {
        transactionCount = 0;
    }

    void displayTransactionHistory() {
        cout << "\n\t\tTransaction History for Account " << accountNum << ":\n";
        for (const string &t : transactions) {
            cout << "\t\t- " << t << "\n";
        }
    }
};

class BankManagement {
private:
    vector<BankAccount> accounts;
    const double savingsInterestRate = 4.5;

public:
    void addAccount(string name, int accountNum, double balance, string password, string type) {
        accounts.push_back(BankAccount(name, accountNum, balance, password, type));
        cout << "\n\t\tAccount Created Successfully!";
    }

    BankAccount* findAccount(int accountNum, string password) {
        for (BankAccount &account : accounts) {
            if (account.getAccountNum() == accountNum) {
                if (account.verifyPassword(password)) {
                    return &account;
                } else {
                    cout << "\n\t\tIncorrect Password!";
                    return nullptr;
                }
            }
        }
        cout << "\n\t\tAccount Not Found!";
        return nullptr;
    }

    void showAllAccounts() {
        cout << "\n\t\tAll Account Holders\n";
        cout << "\t\t-------------------------\n";
        for (const BankAccount &acc : accounts) {
            cout << "\t\tName: " << acc.getName()
                 << "\n\t\tAccount Number: " << acc.getAccountNum()
                 << "\n\t\tAccount Type: " << acc.getAccountType()
                 << "\n\t\tBalance: $" << fixed << setprecision(2) << acc.getBalance() << "\n";
            cout << "\t\t-------------------------\n";
        }
    }

    void applyMonthlyInterest() {
        cout << "\n\t\tApplying monthly interest to all Savings accounts...\n";
        for (BankAccount &acc : accounts) {
            if (acc.getAccountType() == "Savings") {
                acc.addInterest(savingsInterestRate / 12);
            }
        }
    }

    void applyMonthlyPenalties() {
        cout << "\n\t\tApplying monthly penalties to all Current accounts...\n";
        for (BankAccount &acc : accounts) {
            if (acc.getAccountType() == "Current") {
                acc.applyMonthlyPenalties();
            }
        }
    }

    void resetMonthlyTransactionCounts() {
        for (BankAccount &acc : accounts) {
            acc.resetMonthlyTransactions();
        }
    }

    void displayMonthlySummary() {
        cout << "\n\t\tMonthly Summary Report\n";
        showAllAccounts();
    }
};

void displayMenu() {
    cout << "\n\t\t:: Bank Management System ::\n";
    cout << "\t\t1. Create New Account\n";
    cout << "\t\t2. Show All Accounts\n";
    cout << "\t\t3. Search Account\n";
    cout << "\t\t4. Deposit Money\n";
    cout << "\t\t5. Withdraw Money\n";
    cout << "\t\t6. View Transaction History\n";
    cout << "\t\t7. Admin Panel\n";
    cout << "\t\t8. Exit\n";
    cout << "\t\t-------------------------\n";
    cout << "\t\tEnter Your Choice: ";
}

void displayAdminMenu() {
    cout << "\n\t\t:: Admin Panel ::\n";
    cout << "\t\t1. Apply Monthly Interest\n";
    cout << "\t\t2. Apply Monthly Penalties\n";
    cout << "\t\t3. Reset Monthly Transaction Counts\n";
    cout << "\t\t4. Display Monthly Summary Report\n";
    cout << "\t\t5. Back to Main Menu\n";
    cout << "\t\t-------------------------\n";
    cout << "\t\tEnter Your Choice: ";
}

int main() {
    BankManagement bank;
    int choice;
    char op;

    do {
        system("cls");
        displayMenu();
        cin >> choice;
        switch (choice) {
            case 1: {
                string name, password, accountType;
                int accountNum;
                double balance;
                cout << "\t\tEnter Name: ";
                cin >> name;
                getchar();
                cout << "\t\tEnter Account Number: ";
                cin >> accountNum;
                getchar();
                cout << "\t\tEnter Initial Balance: ";
                cin >> balance;
                getchar();
                cout << "\t\tSet Account Password: ";
                cin >> password;
                getchar();
                cout << "\t\tEnter Account Type (Savings/Current): ";
                cin >> accountType;
                bank.addAccount(name, accountNum, balance, password, accountType);
                break;
            }
            case 2:
                bank.showAllAccounts();
                break;
            case 3: {
                int accountNum;
                string password;
                cout << "\t\tEnter Account Number: ";
                cin >> accountNum;
                cout << "\t\tEnter Password: ";
                cin >> password;
                BankAccount *account = bank.findAccount(accountNum, password);
                if (account) {
                    cout << "\t\tName: " << account->getName()
                         << "\n\t\tBalance: $" << fixed << setprecision(2) << account->getBalance() << "\n";
                }
                break;
            }
            case 4: {
                int accountNum;
                double amount;
                string password;
                cout << "\t\tEnter Account Number to Deposit Money: ";
                cin >> accountNum;
                cout << "\t\tEnter Password: ";
                cin >> password;
                BankAccount *account = bank.findAccount(accountNum, password);
                if (account) {
                    cout << "\t\tEnter Amount to Deposit: ";
                    cin >> amount;
                    account->deposit(amount);
                }
                break;
            }
            case 5: {
                int accountNum;
                double amount;
                string password;
                cout << "\t\tEnter Account Number to Withdraw Money: ";
                cin >> accountNum;
                cout << "\t\tEnter Password: ";
                cin >> password;
                BankAccount *account = bank.findAccount(accountNum, password);
                if (account) {
                    cout << "\t\tEnter Amount to Withdraw: ";
                    cin >> amount;
                    account->withdraw(amount);
                }
                break;
            }
            case 6: {
                int accountNum;
                string password;
                cout << "\t\tEnter Account Number: ";
                cin >> accountNum;
                cout << "\t\tEnter Password: ";
                cin >> password;
                BankAccount *account = bank.findAccount(accountNum, password);
                if (account) {
                    account->displayTransactionHistory();
                }
                break;
            }
            case 7: {
                int adminChoice;
                do {
                    displayAdminMenu();
                    cin >> adminChoice;
                    switch (adminChoice) {
                        case 1: bank.applyMonthlyInterest(); break;
                        case 2: bank.applyMonthlyPenalties(); break;
                        case 3: bank.resetMonthlyTransactionCounts(); break;
                        case 4: bank.displayMonthlySummary(); break;
                        case 5: cout << "\n\t\tReturning to Main Menu..."; break;
                    }
                } while (adminChoice != 5);
                break;
            }
            case 8:
                cout << "\n\t\tExiting...";
                exit(0);
            default:
                cout << "\n\t\tInvalid Choice!";
        }
        cout << "\n\t\tDo you want to continue? (Y/N): ";
        cin >> op;
    } while (op == 'Y' || op == 'y');
    return 0;
}