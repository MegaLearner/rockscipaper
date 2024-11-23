#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
using namespace std;

string filename = "happy.txt";

enum permissions {
    all = -1,
    updatee = 2,
    deletee = 4,
    printall = 8,
    finde = 16
};

struct client {
    string accountname;
    string pincode;
    string name;
    string phone;
    double accountbalance;
    bool markfordelete = false;
    permissions perm;
};

vector<string> SplitString(string s1, string delim) {
    vector<string> vstring;
    int pos;
    string sword;
    while ((pos = s1.find(delim)) != string::npos) {
        sword = s1.substr(0, pos);
        if (sword != "") vstring.push_back(sword);
        s1.erase(0, pos + delim.length());
    }
    if (s1 != "") vstring.push_back(s1);
    return vstring;
}

client filldata() {
    client data;
    cout << "Write account name: ";
    cin.ignore();
    getline(cin, data.accountname);
    cout << "Write pincode: ";
    getline(cin, data.pincode);
    cout << "Write name: ";
    getline(cin, data.name);
    cout << "Write phone: ";
    getline(cin, data.phone);
    cout << "Write account balance: ";
    cin >> data.accountbalance;

    int permission_input = 0;
    cout << "Enter permissions for this account (e.g., -1 for all access or sum of values like 2+4+8): ";
    cin >> permission_input;
    data.perm = (permission_input == -1) ? all : static_cast<permissions>(permission_input);

    return data;
}

string recordtoline(client data, string p = "#//#") {
    return data.accountname + p + data.pincode + p + data.name + p + data.phone + p + to_string(data.accountbalance) + p + to_string(data.perm);
}

void saveandwrite(string filename, string dataline) {
    fstream myfile;
    myfile.open(filename, ios::out | ios::app);
    if (myfile.is_open()) {
        myfile << dataline << endl;
        myfile.close();
    }
}

void addnewclient() {
    client data = filldata();
    saveandwrite(filename, recordtoline(data));
}

void printclientrecord(client data) {
    cout << "|" << setw(15) << left << data.accountname
         << "|" << setw(10) << left << data.pincode
         << "|" << setw(40) << left << data.name
         << "|" << setw(12) << left << data.phone
         << "|" << setw(12) << left << data.accountbalance << endl;
}

void printallclients(vector<client> vstring) {
    cout << "Total Clients: " << vstring.size() << endl;
    cout << "|" << setw(15) << left << "Account Number"
         << "|" << setw(10) << left << "Pincode"
         << "|" << setw(40) << left << "Name"
         << "|" << setw(12) << left << "Phone"
         << "|" << setw(12) << left << "Account Balance" << endl;

    for (const client& load : vstring) {
        printclientrecord(load);
    }
}

vector<client> loadclients(string filename) {
    vector<client> vstring;
    string line;
    fstream myfile;
    myfile.open(filename, ios::in);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            vector<string> v = SplitString(line, "#//#");
            if (v.size() >= 6) {
                client data;
                data.accountname = v[0];
                data.pincode = v[1];
                data.name = v[2];
                data.phone = v[3];
                data.accountbalance = stod(v[4]);
                data.perm = static_cast<permissions>(stoi(v[5]));
                vstring.push_back(data);
            }
        }
        myfile.close();
    }
    return vstring;
}

bool findaccount(string accountnumber, vector<client> vstring, client& data) {
    for (const client& c : vstring) {
        if (c.accountname == accountnumber) {
            data = c;
            return true;
        }
    }
    return false;
}

bool login(string accountname, string password, vector<client> vstring, client& data) {
    for (const client& c : vstring) {
        if (c.accountname == accountname && c.pincode == password) {
            data = c;
            return true;
        }
    }
    return false;
}

bool markfordelete(string accountnumber, vector<client>& data) {
    for (client& c : data) {
        if (c.accountname == accountnumber) {
            c.markfordelete = true;
            return true;
        }
    }
    return false;
}

vector<client> saveclienttofile(string filename, vector<client> vstring) {
    fstream myfile;
    myfile.open(filename, ios::out);
    if (myfile.is_open()) {
        for (const client& c : vstring) {
            if (!c.markfordelete) {
                myfile << recordtoline(c) << endl;
            }
        }
        myfile.close();
    }
    return vstring;
}

bool deleteclientbyaccount(string accountnumber, vector<client>& vstring) {
    client data;
    char answer = 'n';
    if (findaccount(accountnumber, vstring, data)) {
        printclientrecord(data);
        cout << "Are you sure you want to delete this (y/n)? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            markfordelete(accountnumber, vstring);
            saveclienttofile(filename, vstring);
            cout << "Deleted successfully.\n";
            return true;
        }
    }
    cout << "Client account number " << accountnumber << " not found.\n";
    return false;
}

client changerecord(string accountnumber) {
    client data;
    data.accountname = accountnumber;
    cout << "Write pincode: ";
    cin.ignore();
    getline(cin, data.pincode);
    cout << "Write name: ";
    getline(cin, data.name);
    cout << "Write phone: ";
    getline(cin, data.phone);
    cout << "Write account balance: ";
    cin >> data.accountbalance;
    return data;
}

bool update(string accountnumber, vector<client>& vstring) {
    client data;
    char answer = 'n';
    if (findaccount(accountnumber, vstring, data)) {
        printclientrecord(data);
        cout << "Are you sure you want to update this (y/n)? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            for (client& c : vstring) {
                if (c.accountname == accountnumber) {
                    c = changerecord(accountnumber);
                    break;
                }
            }
            saveclienttofile(filename, vstring);
            cout << "Updated successfully.\n";
            return true;
        }
    }
    cout << "Client not found.\n";
    return false;
}
void simplegame(client& loggedInClient) {
    const vector<string> choices = {"Rock", "Paper", "Scissors"};
    int playerWins = 0, computerWins = 0;
    srand(time(0)); // Seed random number generator

    cout << "Welcome to Rock, Paper, Scissors! Beat the computer in 3 out of 5 rounds to win 10 coins.\n";

    for (int round = 1; round <= 5; ++round) {
        int computerChoice = rand() % 3; // 0: Rock, 1: Paper, 2: Scissors
        string playerChoice;
        cout << "\nRound " << round << " - Enter your choice (Rock, Paper, Scissors): ";
        cin >> playerChoice;

       
        transform(playerChoice.begin(), playerChoice.end(), playerChoice.begin(), ::tolower);

      
        int playerIndex = -1;
        if (playerChoice == "rock") playerIndex = 0;
        else if (playerChoice == "paper") playerIndex = 1;
        else if (playerChoice == "scissors") playerIndex = 2;
        else {
            cout << "Invalid choice! Computer wins this round.\n";
            ++computerWins;
            continue;
        }

        cout << "Computer chose: " << choices[computerChoice] << "\n";

      
        if (playerIndex == computerChoice) {
            cout << "It's a tie!\n";
        } else if ((playerIndex == 0 && computerChoice == 2) || 
                   (playerIndex == 1 && computerChoice == 0) || 
                   (playerIndex == 2 && computerChoice == 1)) { 
            cout << "You win this round!\n";
            ++playerWins;
        } else {
            cout << "Computer wins this round!\n";
            ++computerWins;
        }

      
        if (playerWins == 3 || computerWins == 3) break;
    }

    if (playerWins >= 3) {
        cout << "\nCongratulations! You beat the computer and earned 10 coins!\n";
        loggedInClient.accountbalance += 10;
    } else {
        cout << "\nBetter luck next time! The computer won.\n";
    }

    cout << "Your total account balance: " << loggedInClient.accountbalance << " coins.\n";
}
void choose(int x, client& loggedInClient) {
    string accountn;
    vector<client> vstring = loadclients(filename);

    if (x == 1 && (loggedInClient.perm == all)) {
        addnewclient();
    } else if (x == 2 && (loggedInClient.perm & updatee)) {
        cout << "Write your account number: ";
        cin >> accountn;
        update(accountn, vstring);
    } else if (x == 3 && (loggedInClient.perm & deletee)) {
        cout << "Write your account number: ";
        cin >> accountn;
        deleteclientbyaccount(accountn, vstring);
    } else if (x == 4 && (loggedInClient.perm & printall)) {
        printallclients(vstring);
    } else if (x == 5 && (loggedInClient.perm & finde)) {
        cout << "Write your account number: ";
        cin >> accountn;
        client foundClient;
        if (findaccount(accountn, vstring, foundClient))
            printclientrecord(foundClient);
        else
            cout << "Account not found.\n";
    } else if (x == 6) {
        simplegame(loggedInClient);

        // Update the `loggedInClient` balance back into `vstring`
        for (client& c : vstring) {
            if (c.accountname == loggedInClient.accountname) {
                c.accountbalance = loggedInClient.accountbalance;
                break;
            }
        }

        // Save the updated clients back to the file
        saveclienttofile(filename, vstring);
    } else if (x == 7) {
        return; // Logout
    } else {
        cout << "You do not have permission to perform this action.\n";
    }

    // Save any updates made during other operations
    saveclienttofile(filename, vstring);
}

int readoption() {
    int x;
    cin >> x;
    return x;
}

bool showscreen(client loggedInClient) {
    cout << "\nChoose an option:\n";
    if (loggedInClient.perm == all) cout << "1. Add record\n";
    if (loggedInClient.perm & updatee) cout << "2. Update record\n";
    if (loggedInClient.perm & deletee) cout << "3. Delete record\n";
    if (loggedInClient.perm & printall) cout << "4. Print all client data\n";
    if (loggedInClient.perm & finde) cout << "5. Find account\n";
    cout << "6. Play Rock, Paper, Scissors\n";
    cout << "7. Logout\n";

    int choice = readoption();
    choose(choice, loggedInClient);
    return choice == 7;
}

void login_screen() {
    string accountnumber, pincode;
    vector<client> vstring = loadclients(filename);
    int x;
    cout << "1. Login\n2. Register\nChoose an option: ";
    cin >> x;
    switch (x) {
        case 1: {
            cout << "Enter account number: ";
            cin >> accountnumber;
            cout << "Enter pincode: ";
            cin >> pincode;
            client data;
            if (login(accountnumber, pincode, vstring, data)) {
                while (!showscreen(data));
            } else cout << "Invalid account number or pincode.\n";
            break;
        }
        case 2: addnewclient(); break;
        default: cout << "Invalid option.\n"; break;
    }
}

int main() {
    while (true) login_screen();
    return 0;
}
