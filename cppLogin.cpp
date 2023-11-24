#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <sstream> // Include for istringstream

using namespace std;

unsigned int hashString(const string &str) {
    unsigned int hash = 0;
    for (char ch : str) {
        hash = hash * 121212139 + ch;
    }
    return hash;
}

void registerUser() {
    string username, password;
    cout << "Enter a username: ";
    cin >> username;

    ifstream file("users.txt");
    string line;
    bool usernameExists = false;
    while (getline(file, line)) {
        size_t pos = line.find(' ');
        if (pos != string::npos) {
            string storedUsername = line.substr(0, pos);
            if (username == storedUsername) {
                usernameExists = true;
                break;
            }
        }
    }
    file.close();

    if (usernameExists) {
        cout << "Username already exists. Please choose a different username." << endl;
    } else {
        cout << "Enter a password: ";
        cin >> password;

        unsigned int hashedPassword = hashString(password);

        ofstream outFile("users.txt", ios::app);
        outFile << username << " " << hashedPassword << endl;
        outFile.close();

        cout << "Registration successful! Hashed Password: " << hashedPassword << endl;
    }
}

bool loginUser() {
    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    unsigned int hashedPassword = hashString(password);

    ifstream file("users.txt");
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(' ');
        if (pos != string::npos) {
            string storedUsername = line.substr(0, pos);
            string hashedPasswordStr = line.substr(pos + 1);

            try {
                unsigned int storedHashedPassword;
                istringstream(hashedPasswordStr) >> storedHashedPassword;

                if (username == storedUsername && hashedPassword == storedHashedPassword) {
                    file.close();
                    return true;
                }
            } catch (const std::invalid_argument& e) {
                cerr << "Error: Invalid password format in the file." << endl;
                file.close();
                return false;
            } catch (const std::out_of_range& e) {
                cerr << "Error: Out of range exception during password conversion." << endl;
                file.close();
                return false;
            }
        }
    }
    file.close();

    return false;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void showSpinner(int durationInSeconds) {
    cout << "Logging in";
    for (int i = 0; i < durationInSeconds; ++i) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << endl;
}

int main() {
    int choice;

    do {
        cout << "Choose an option:" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    cout << "Login successful!" << endl;
                    showSpinner(2);
                    clearScreen();
                    cout << "Hello, World!" << endl;
                    return 0;
                } else {
                    cout << "Invalid username or password. Please try again." << endl;
                }
                break;
            case 0:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (choice != 0);
    return 0;
}
