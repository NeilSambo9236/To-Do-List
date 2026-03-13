#include <cctype>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

void inputToDo(int& limit, int number, int order, string toDo, vector<string>& toDoList);

void showList(int number, string toDo) { // Prints all the content of the to-do list
    cout << "Here is your to-do list: " << endl;

    cout << endl;

    ifstream file5("toDoList.txt");

    while (file5 >> number >> toDo) {
        cout << number << ". " << toDo << endl;
    }

    file5.close();
}

void remove(int& limit, int number, int order, string toDo, vector<string>& toDoList) {
    char answer;
    bool notFound = false;

    cout << "Enter the number of the task you want to remove: ";
    cin >> number;

    while (cin.fail()) {
        cout << "Invalid Input. Please try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> number;
    }
    
    while (!notFound) { // Handles the wrong number order to find the to-do in the list and asks again
        ifstream file3("toDoList.txt");

        while (file3 >> order) {
            getline(file3, toDo);
            if (number == order) {
                notFound = true;

                string orderString = to_string(order); // String to int convertion

                orderString.clear(); // Removes the selected to-do
                toDo.clear();
            }

            if (isspace(toDo[0]) && !(toDo.empty())) {
                toDo.erase(0, 1); // Removes the space at the start of a to-do
                toDoList.push_back(toDo); // Checks and ignores the removed to-do
            }
        }

        file3.close();

        if (number > order) { // If the number is not found, ask again
            cout << "Invalid order. Please try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> number;
        }

        else // If the number is found in the list, break the loop
            break;

    }

    ofstream file4("toDoList.txt", ios::trunc); // Deletes the content of the list file

    file4.close();

    inputToDo(limit, number, order, toDo, toDoList); // Calls the function to show the list
}

void inputToDo(int& limit, int number, int order, string toDo, vector<string>& toDoList) {
    ifstream file3("toDoList.txt");

    while (file3 >> order >> toDo) { // Finds the largest number in the list. If none, initializes it to 1
        if (order > limit) {
            order += 1;
            limit = order;
        }
    }

    file3.close();

    ofstream file("toDoList.txt", ios::app);

    for (int i = 0; i < toDoList.size(); i++) { // Puts the to-dos in the toDoList file
        file << limit + i << " " << toDoList[i] << endl;
    }

    file.close();

    showList(number, toDo); // Calls the function to show the list
}

int main () {
    int choice, number, order, removeResult;  
    int limit = 1;

    string toDo;
    vector<string> toDoList; // Vector

    cout << "To do list.\n" << endl;
    cout << "You can add the things you want/need to do." << endl;
    cout << "You can remove them if you're done or you made mistake." << endl;

    cout << "\nClick 1 to input to-dos, 2 to remove, and 3 to check your to-do list: ";
    cin >> choice;

    while (cin.fail() || choice > 3 || choice < 1) {
        cout << "Invalid answer. Please input 1, 2 or 3: ";
        cin.clear();
        cin.ignore(100000000, '\n');
        cin >> choice;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // To remove space for getline

    cout << endl;

    if (choice == 1) {
        while (true) {
        cout << "Enter the to-do you want to input(input 0 to stop): "; // Enter multiple to-dos in vector
        getline(cin, toDo);

        if (toDo == "0") break; // Sentinel

        toDoList.push_back(toDo);
        }

        inputToDo(limit, number, order, toDo, toDoList);
    }

    else if (choice == 2) {
        remove(limit, number, order, toDo, toDoList);
    }

    else 
        showList(number, toDo);
}