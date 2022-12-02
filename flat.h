#ifndef FLAT_H
#define FLAT_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
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

public:
    Flat()
    {
        int room_no = 101;
        int rent = 4199, prev = room_no,floor = 0 ,ctr=0;
        for (size_t i = 0; i < 40; i++)
        {
            flats.push_back(vector<string>());
            flats[i].push_back(to_string(floor+1));
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
        cout << "Available Flats are :";
        int prev_floor = stoi(flats[0][0]);
        cout << "\nFloor : " << prev_floor << endl;
        for (size_t i = 0; i < flats.size(); i++)
        {
            //if floor is same then don't display floor again.
            if (prev_floor != stoi(flats[i][0]) && flats[i][3].compare("NBKD") == 0 | flats[i][3].compare("On Sale") == 0)
                cout << "\nFloor : " << flats[i][0] << endl;
            //display only not booked or onsale flats
            if (flats[i][3].compare("NBKD") == 0 | flats[i][3].compare("On Sale") == 0)
                cout << "Room no : " << flats[i][1] << "\t\tRent :" << flats[i][2] << "\t\tStatus : " << flats[i][3] << endl;
            prev_floor = stoi(flats[i][0]);
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
            string command = "start " + flats[i][flats[i].size()-1];
#elif __linux__
            string command = "xdg-open " + flats[i][flats[i].size()-1];
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
            if (room_no == stoi(flats[i][1]) && (flats[i][3].compare("NBKD") == 0 | flats[i][3].compare("On Sale") == 0))
            {
                flat_available = true;
                break;
            }
        }
        if (flat_available) // flat is available
        {
            long long aadhar;
            cout << "Enter your Aadhar number : ";
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
                    getline(cin,nme);
                    cout << "Enter your Permanent Address : ";
                    getline(cin,add);
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
        else
        {
            cout << "Flat number " << room_no << " not available.\n";
        }
    }
    void sellFlat(int room_no)
    {
        bool flat_available = false;
        int i; // to store the row in which flat details are stored
        string nme, add;

        for (i = 0; i < flats.size(); i++)
        {
            if (room_no == stoi(flats[i][1]) && (flats[i][3].compare("BKD") == 0))
            {
                nme = flats[i][4];
                add = flats[i][6];
                flat_available = true;
                break;
            }
        }
        if (flat_available)
        {
            long long aadhar;
            cout << "Enter your Aadhar number : ";
            cin >> aadhar;
            if (to_string(aadhar).length() == 12)
            {
                cout << "Details for flats are : \n";
                cout << "Room no : " << room_no << "\tName : " << nme << "\tAadhar : " << aadhar << "\tAddress : " << add << ".\nDo you want to sell this Flat ?(y/n) :";
                char ch;
                cin >> ch;
                if (ch == 'y')
                {
                    // add details of flat.
                    flats[i][3] = "NBKD";
                    flats[i][4] = "NA";
                    flats[i][5] = "NA";
                    flats[i][6] = "NA";
                    cout << "Flat Sold!\n";
                    writeFile();
                }
                cout << "\nHave a nice and peaceful day !.\nCome Again Soon.\n";
            }
            else
                cout << "Invalid Aadhar number\n";
        }
        else
            cout << "Enter Correct Flat Number!\n";
    }
};
#endif