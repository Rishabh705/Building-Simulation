#ifndef FLAT_H
#define FLAT_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
using namespace std;
class Flat
{                                 //   0      1      2     3     4    5        6
    vector<vector<string>> flats; // Floor, Room no,rent,status,name,aadhar,address.
    // Write changes to file from vector flats.
    void writeFile()
    {
        fstream fio;
        string line, word;
        fio.open("flats.csv", ios::out);
        int i = 0;
        while (fio && i < flats.size())
        {
            for (size_t j = 0; j < flats[i].size(); j++)
            {
                word = flats[i][j];
                word += ", ";
                fio << word;
            }
            i++;
            fio << "\n";
        }
        fio.close();
    }
    vector<vector<string>> customers_of_library;
    void readFile()
    {
        fstream fio;
        string line, word;
        fio.open("library_members.csv", ios::in | ios::out);
        fio.seekg(214, ios::beg);
        customers_of_library.push_back(vector<string>(0)); // aadhar
        customers_of_library.push_back(vector<string>(0)); // amount owed
        int i = 1;
        while (fio >> word)
        {
            word = word.substr(0, word.length() - 1);
            if (i == 2)
            {
                customers_of_library[0].push_back(word);
            }
            else if (i == 6)
            {
                customers_of_library[1].push_back(word);
            }
            i++;
        }
        fio.close();
    }

public:
    Flat()
    {
        int room_no = 101;
        int rent = 4199, prev = room_no, floor = 0, ctr = 0;
        for (size_t i = 0; i < 40; i++)
        {
            flats.push_back(vector<string>());
            flats[i].push_back(to_string(floor + 1));
            flats[i].push_back(to_string(room_no));
            flats[i].push_back(to_string(rent));
            flats[i].push_back("NBKD");
            flats[i].push_back("NA");
            flats[i].push_back("NA");
            flats[i].push_back("NA");
            if (ctr == 0)
                flats[i].push_back("./pics/flat1.png");
            else if (ctr == 1)
                flats[i].push_back("./pics/flat2.png");
            else if (ctr == 2)
                flats[i].push_back("./pics/flat3.png");
            else
                flats[i].push_back("./pics/flat4.png");
            room_no++;
            rent += 500;
            if (ctr == 3)
            {
                room_no = prev + 100;
                prev = room_no;
                ctr = 0;
                rent = 4199;
                floor++;
            }
            else
                ctr++;
        }
    }
    void showAvailableFlats()
    {
        int ctr = 1;
        cout << "Available Flats are :";
        for (size_t i = 0; i < flats.size(); i++)
        {
            // // display only not booked
            if (ctr == 1)
                cout << "\nFloor : " << flats[i][0] << endl;
            if (flats[i][3].compare("NBKD") == 0)
                cout << "Room no : " << flats[i][1] << "\t\tRent :" << flats[i][2] << "\t\tStatus : " << flats[i][3] << endl;
            if (ctr == 4)
                ctr = 1;
            else
                ctr++;
        }
    }
    void showFlat(int room_no)
    {
        int i;
        bool found = false;
        for (i = 0; i < flats.size(); i++)
        {
            if (room_no == stoi(flats[i][1]))
            {
                found = true;
                break;
            }
        }
        if (found)
        {
#ifdef _WIN64
            string command = "start " + flats[i][flats[i].size() - 1];
#elif __linux__
            string command = "xdg-open " + flats[i][flats[i].size() - 1];
#endif
            system(command.c_str());
        }
        else
        {
            cout << "No such Flat present!\n";
        }
    }
    void bookFlat(int room_no)
    {
        bool flat_available = false;
        int i, j;
        for (i = 0; i < flats.size(); i++)
        {
            if (room_no == stoi(flats[i][1]) && (flats[i][3].compare("NBKD") == 0))
            {
                flat_available = true;
                break;
            }
        }
        if (flat_available) // flat is available
        {
            long long aadhar;
            cout << "Enter your Aadhar number : ";
            try
            {
                cin >> aadhar;
                if (to_string(aadhar).length() == 12)
                {
                    // demand rent
                    float rent = stoi(flats[i][2]); // map it with room so as to have different rates.
                    cout << "Rent for Room no." << room_no << " is : " << rent << ". No bargain !\nDo you want to buy this Flat ?(y/n) :";
                    char ch;
                    cin >> ch;
                    if (ch == 'y')
                    {
                        // add details of flat.
                        string nme, add;
                        cout << "Enter your Name : ";
                        cin.ignore();
                        getline(cin, nme);
                        cout << "Enter your Permanent Address : ";
                        getline(cin, add);
                        flats[i][3] = "BKD";
                        flats[i][4] = nme;
                        flats[i][5] = to_string(aadhar);
                        flats[i][6] = add;
                        cout << "Flat Booked!\n";
                        writeFile();
                    }
                    cout << "\nHave a nice and peaceful day !\n";
                }
                else
                    cout << "Invalid Aadhar number\n";
            }
            catch (...)
            {
                cout << "Invalid Aadhar number\n";
            }
            cout << "This terminal will be erased shortly\n";
            sleep(2);
            int system_return_val = system("cls");
            if (system_return_val != 0)
            {
                system("clear");
            }
        }
        else
        {
            cout << "Flat number " << room_no << " not available.\n";
        }
    }
    void sellFlat(int room_no)
    {
        long long aadhar;
        cout << "Enter your Aadhar number : ";
        try
        {
            cin >> aadhar;
            if (to_string(aadhar).length() == 12)
            {
                bool flat_booked = false;
                int i; // to store the row in which flat details are stored
                string nme, add;
                for (i = 0; i < flats.size(); i++)
                {
                    if (room_no == stoi(flats[i][1]) && (flats[i][3].compare("BKD") == 0) && (flats[i][5].compare(to_string(aadhar)) == 0))
                    {
                        nme = flats[i][4];
                        add = flats[i][6];
                        flat_booked = true;
                        break;
                    }
                }
                if (flat_booked)
                {
                    readFile();
                    bool library_member = false;
                    bool fine_cleared = false;
                    for (size_t j = 0; j < customers_of_library[0].size(); j++)
                    {
                        if (customers_of_library[0][j].compare(to_string(aadhar)) == 0) // if flat customer is also customer of library
                        {
                            library_member = true;                            // customer is member of library
                            if (customers_of_library[1][j].compare("0") == 0) // if fine is 0
                                fine_cleared = true;
                            break;
                        }
                    }
                    cout << "Details for flats are : \n";
                    cout << "Room no : " << room_no << "\tName : " << nme << "\tAadhar : " << aadhar << "\tAddress : " << add << ".\nDo you want to sell this Flat ?(y/n) :";
                    char ch;
                    cin >> ch;
                    if (ch == 'y' | ch == 'Y')
                    {
                        if ((library_member == true && fine_cleared == true) | !library_member)
                        { // add details of flat.
                            flats[i][3] = "NBKD";
                            flats[i][4] = "NA";
                            flats[i][5] = "NA";
                            flats[i][6] = "NA";
                            cout << "Flat Sold!\n";
                            writeFile();
                            cout << "\nHave a nice and peaceful day !.\nCome Again Soon.\n";
                        }
                        else
                            cout << "Clear your fine in library first!\n";
                    }
                    else
                        cout << "Have a nice day.\n";
                }
                else
                    cout << "Flat not available/aadhar unregistered!\n";
                cout << "This terminal will be erased shortly\n";
                sleep(2);
                int system_return_val = system("cls");
                if (system_return_val != 0)
                {
                    system("clear");
                }
            }
            else
                cout << "Invalid Aadhar number\n";
        }
        catch(...)
        {
            cout << "INVALID SYNTAX\n";
        }
    }
};
#endif