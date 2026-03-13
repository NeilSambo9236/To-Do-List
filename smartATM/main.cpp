#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using std::cout; // To not include std on different stream objects
using std::cin;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ios;

void fileEdit(string target, int withdrawal, int deposit) { // Rewrites the file for changes because of withdrawal, deposit, check balance
    string username, password;
    int balance;

    ofstream file4("dummy.txt"); // Makes dummy text file
    ifstream file5("accounts.txt"); // Opens the existing accounts text file

    while (file5 >> username >> password >> balance) { // Reads the entire file and finds your account
        if (target == username) {
            if (withdrawal > 0) { // Withdrawal calculation
                balance -= withdrawal;
                cout << "Withdrawal complete! Your current balance is " << balance << endl;
            }
            else if (deposit > 0) { // Deposit calculation
                balance += deposit;
                cout << "Deposit complete! Your current balance is " << balance << endl;
            }
            else cout << "Your current balance is " << balance << endl; // Checks balance
        }
        file4 << username << " " << password << " " << balance << endl; // Prints all the accounts in the dummy text file
    }

    file4.close(); // Closes files
    file5.close();

    remove("accounts.txt"); // Deletes the existing accounts text file
    rename("dummy.txt", "accounts.txt"); // Makes the dummy text file to be the original or new one to be used later on
}

void machineProcess(string username, string password, int balance) { // The ATM machine
    int answer, deposit, withdrawal;
    int accountBalance = 0;
    string fileTarget = username;

    do { // Loops the ATM machine until the user wants it to stop via clicking 0

        cout << endl;

        cout << "Welcome to the ATM machine. Input 0 to end program, 1 for withdrawing, 2 for depositing, and 3 for checking your balance: ";
        cin >> answer;

        while (cin.fail() || answer < 0 || answer > 3) {
            cout << "Invalid input. Please try again: ";
            cin.clear();
            cin.ignore(10000000, '\n');
            cin >> answer;
        }

        ifstream fileNew("accounts.txt"); // To get the balance of the specific account, mainly for input validation in withdrawal

        while (fileNew >> username >> password >> balance) {
            if (fileTarget == username) {
                accountBalance = balance;
            }
        }

        fileNew.close();

        if (answer == 1) { // Withdrawal system
            cout << "Enter the amount you want to withdraw: ";
            cin >> withdrawal;

            while (cin.fail() || withdrawal < 1 || withdrawal > accountBalance) {
                cout << "Invalid amount. Please try again: ";
                cin.clear();
                cin.ignore(10000000, '\n');
                cin >> withdrawal;
            }
            fileEdit(fileTarget, withdrawal, 0);
        }

        else if (answer == 2) { // Deposit system
            cout << "Enter the amount you want to deposit: ";
            cin >> deposit;

            while (cin.fail() || deposit < 1) {
                cout << "Invalid amount. Please try again: ";
                cin.clear();
                cin.ignore(100000000, '\n');
                cin >> deposit;
            }
            fileEdit(fileTarget, 0, deposit);
        }

        else if (answer == 3) { // Checking balance
            fileEdit(fileTarget, 0, 0);
        }

        else { // Message when the program ends
            cout << "Thank you for using the ATM machine!" << endl;
        }
    } while (answer != 0);
}

int main () {
    string username, password, targetUsername, targetPassword;
    int balance, logInAnswer;
    bool target = false;

    int maxUsernameAttempts = 0;
    int maxPasswordAttempts = 0;

    cout << "Would you like to log-in or sign-up?(1 for log-in and 2 for sign-up): ";
    cin >> logInAnswer;

    while (cin.fail() || logInAnswer > 2 || logInAnswer < 1) {
        cout << "Invalid answer. Please input 1 or 2: ";
        cin.clear();
        cin.ignore(1000000, '\n');
        cin >> logInAnswer;
    }

    if (logInAnswer == 1) { // Log-in to an existing account

        cout << "Enter your username: ";
        cin >> targetUsername;

        while (maxUsernameAttempts < 5) { // Maximum attempts of username identification is 5

            ifstream file("accounts.txt");

            while (file >> username >> password >> balance) { // Reads the entire file and finds your account
                if (targetUsername == username) {
                    file.close();
                    target = true;
                    cout << "Enter your password: ";
                    cin >> targetPassword;

                    while (maxPasswordAttempts < 5) { // Maximum attempts of password identification is 5
                        if (targetPassword == password) {
                            cout << "Log-in successful!" << endl;
                            machineProcess(username, password, balance); // Moves on the ATM machine if log-in is successful
                            break;
                        }
                        else { // Invalid password input handling
                            cout << "Invalid password. Please try again: ";
                            cin.clear();
                            cin.ignore(1000000, '\n');
                            maxPasswordAttempts++;
                            cin >> targetPassword;
                        }
                    }
                    if (maxPasswordAttempts == 5) {
                        cout << "Maximum attempts reached. Please try again later." << endl;
                    }
                return 0; // Exists when username and password identification was reached
                }
            }

            file.close();

            if (!target) { // Invalid username input handling
                cout << "Invalid username. Please try again: ";
                cin.clear();
                cin.ignore(1000000, '\n');
                maxUsernameAttempts++;
                cin >> targetUsername;
            }
        }
        if (maxUsernameAttempts == 5) {
            cout << "Maximum attempts reached. Please try again later." << endl;
        }
    }

    else { // Sign-in. Making a new account
        cout << "Enter your desired username: ";
        cin >> targetUsername;

        ifstream file2("accounts.txt");

        while (file2 >> username >> password >> balance) {
            while (targetUsername == username) {
                cout << "Username is already used. Please input another one: ";
                cin.clear();
                cin.ignore(100000, '\n');
                cin >> targetUsername;
            }
        }

        file2.close();

        cout << "Enter your desired password: ";
        cin >> password;

        cout << "Enter your balance: ";
        cin >> balance;

        while (cin.fail() || balance < 1) {
            cout << "Invalid balance. Please try again: ";
            cin.clear();
            cin.ignore(10000000, '\n');
            cin >> balance;
        }

        ofstream file3("accounts.txt", ios::app); // Makes the file and puts new accounts

        file3 << targetUsername << " " << password << " " << balance << endl;

        file3.close();

        cout << "Account creation complete!" << endl;
        cout << endl;

        machineProcess(targetUsername, password, balance); // Moves on to the ATM machine if sign-up is successful
    }
}