#ifndef PARKING_H
#define PARKING_H
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <bits/stdc++.h>
#define MAX 50
using namespace std;
int j = 0;
string lower(string str1)
{
    for (int i = 0; i < str1.length(); i++)
        str1[i] = tolower(str1[i]);
    return str1;
}
int countWords(string line)
{
    stringstream stream(line);
    return distance(istream_iterator<string>(stream), istream_iterator<string>());
}
class Valid
{
protected:
    bool checkNumber(string reg) // check state codes
    {
        string statecodes[] = {"AN", "AP", "AR", "AS", "BH", "BR", "CH", "CG", "DD", "DL, GA",
                               "GJ", "HR", "HP", "JK", "JH", "KA", "KL", "LA", "LD", "MP", "MH",
                               "MN", "ML", "MZ", "NL", "OD", "PY", "PB", "RJ", "SK", "TN", "TS",
                               "TR", "UP", "UK", "WB"};
        if (reg.length() == 9)
        {
            if (isdigit(reg[0]) && isdigit(reg[1]) && isdigit(reg[4]) && isalpha(reg[2]) && isalpha(reg[3]) && isalpha(reg[5]) && isalpha(reg[6]) && isalpha(reg[7]) && isalpha(reg[8]))
                return false;
            for (size_t i = 0; i < 37; i++)
                if (((lower(statecodes[i]).compare(lower(reg.substr(0, 2)))) == 0))
                    return true;
            return false;
        }
        else if (reg.length() == 10)
        {
            if (isdigit(reg[0]) && isdigit(reg[1]) && isdigit(reg[4]) && isdigit(reg[5]) && isalpha(reg[2]) && isalpha(reg[3]) && isalpha(reg[6]) && isalpha(reg[7]) && isalpha(reg[8]) && isalpha(reg[9]))
                return false;
            for (size_t i = 0; i < 37; i++)
                if (((lower(statecodes[i]).compare(lower(reg.substr(0, 2)))) == 0))
                    return true;
            return false;
        }
        return false;
    }
    bool checkColour(string clr)
    {
        string colour[10] = {"red", "blue", "yellow", "green", "silver", "black", "white", "grey", "orange", "purple"};
        for (size_t i = 0; i < 12; i++)
            if (((lower(clr).compare(lower(colour[i]))) == 0 | lower(clr).compare(lower(colour[i])) == 0))
                return true;
        return false;
    }
};
class ParkingEntry : public Valid
{
    vector<int> empty_slots;
    void writeFile()
    {
        fstream fio;
        string line, word;
        fio.open("parking.csv", ios::out);
        int i = 0;
        while (fio && i < slots.size())
        {
            for (size_t j = 0; j < slots[i].size(); j++)
            {
                word = slots[i][j];
                word += ", ";
                fio << word;
            }
            i++;
            fio << "\n";
        }
        fio.close();
    }

protected:
    vector<vector<string>> slots;

    bool exists(string reg)
    {
        for (size_t i = 0; i < slots.size(); i++)
        {
            if (reg.compare(slots[i][0]) == 0)
                return true;
        }
        return false;
    }

    ParkingEntry()
    {
        for (size_t i = 0; i < MAX; i++)
            empty_slots.push_back(i); // as initially all slots are empty.
    }
    void park(string n, string clr, string pass) // park vehicle of particular color = clr  and number = n at nearest slot.
    {
        if (checkNumber(n) && checkColour(clr))
        {
            if (!exists(n))
            {
                if (j < MAX)
                {
                    slots.push_back(vector<string>());                                  // make a row to take data
                    int nearest = *min_element(empty_slots.begin(), empty_slots.end()); // nearest available slot obtained
                    slots[j].push_back(n);
                    slots[j].push_back(clr);
                    slots[j].push_back(to_string(nearest + 1));
                    slots[j].push_back(pass);
                    auto it = find(empty_slots.begin(), empty_slots.end(), nearest);
                    int index = it - empty_slots.begin();
                    empty_slots.erase(empty_slots.begin() + index);
                    cout << "Your vehicle " << n << " is parked at slot number " << nearest + 1 << endl;
                    j++;
                    cout << "Press enter to clear terminal\n";
                    string temp;
                    getline(cin >> ws, temp);
                    int system_return_val = system("cls");
                    if (system_return_val != 0)
                    {
                        system("clear");
                    }
                    writeFile();
                }
                else
                    cout << "Sorry...Parking Lot is full!\n";
            }
            else
            {
                cout << "Car " << n << " already parked.\n";
            }
        }
        else
            cout << "Incorrect data!!\n";
    }
    void unpark(string n, string pass)
    {
        bool vehicle_was_present = false;
        int i;
        for (i = 0; i < j; i++)
        {
            if (n.compare(slots[i][0]) == 0 && pass.compare(slots[i][3]) == 0)
            {
                slots.erase(slots.begin() + i);
                cout << "Your vehicle " << n << " is unparked!\n";
                vehicle_was_present = true;
                j--;
                writeFile();
                cout << "Press enter to clear terminal\n";
                string temp;
                getline(cin >> ws, temp);
                int system_return_val = system("cls");
                if (system_return_val != 0)
                {
                    system("clear");
                }
                break;
            }
        }
        if (vehicle_was_present)
            empty_slots.push_back(i);
        else
            cout << "No such vehicle present.\n";
    }
};
class Details : public ParkingEntry
{
    string command;
    string commands[4];
    void findVehicle(string reg)
    {
        bool found = false;
        for (size_t i = 0; i < j; i++)
        {
            if (reg.compare(slots[i][0]) == 0)
            {
                cout << "\nRegistration number\tSlot number\tColour\n";
                cout << "     " << reg << "\t\t   " << stoi(slots[i][2]) + 1 << "\t\t " << slots[i][1] << endl;
                found = true;
                break;
            }
        }
        if (found == false)
            cout << "No data found.\n";
    }
    void displayCommands()
    {
        cout << "Kindly enter one of the following commands to execute the corresponding task" << endl;
        cout << "1. park <number_plate> <color> <pin (your choice)>                /* To park your vehicle */" << endl;
        cout << "2. unpark <number_plate> <registered pin>                         /* Remove the vehicle from parking slot */" << endl;
        cout << "3. findVehicle <number_plate>                                     /* Find your vehicle */" << endl;
        cout << "4. clear                                                          /* clear terminal */" << endl;
        cout << "5. help                                                           /* To display commands again */" << endl;
        cout << "6. exit                                                           /* To exit library */" << endl;
    }
    void inputCommands()
    {
        int index = 0;
        cout << "\nEnter command: ";
        getline(cin >> ws, command);
        commands[0] = commands[1] = commands[2] = commands[3] = "";
        for (int i = 0; (i < command.length()) && (index < 4); i++)
        {
            if (command[i] == ' ')
            {
                index++;
            }
            else
            {
                commands[index] += command[i];
            }
        }
    }

public:
    void enterParkingLot()
    {
        cout << "Welcome to parking lot\n\n";
        displayCommands();
        while (true)
        {
            inputCommands();
            transform(commands[0].begin(), commands[0].end(), commands[0].begin(), ::tolower);
            if (commands[0] == "exit" || commands[0] == "end" || commands[0] == "quit")
            {
                cout << "Exiting Parking lot...\n";
                break;
            }
            else if (commands[0] == "park")
            {
                try
                {
                    if (countWords(command) == 4)
                        park(commands[1], commands[2], commands[3]);
                    else
                        throw(0);
                }
                catch (...)
                {
                    cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                }
            }
            else if (commands[0] == "unpark")
            {
                try
                {
                    if (countWords(command) == 3)
                        unpark(commands[1], commands[2]);
                    else
                        throw(0);
                }
                catch (...)
                {
                    cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                }
            }
            else if (commands[0] == "findvehicle")
            {
                try
                {
                    if (countWords(command) == 2)
                        findVehicle(commands[1]);
                    else
                        throw(0);
                }
                catch (...)
                {
                    cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                }
            }
            else if (commands[0] == "clear")
            {
                int system_return_val = system("cls");
                if (system_return_val != 0)
                {
                    system("clear");
                }
            }
            else if (commands[0] == "help")
            {
                displayCommands();
            }
            else
            {
                cout << "Invalid command\nTry again\n";
            }
        }
        cout << "Parking Lot Exited\n\n";
    }
};
#endif