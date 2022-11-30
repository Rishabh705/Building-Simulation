#include <iostream>
#include <algorithm>
#include "game.h"
#include "flat.h"
#include "parking.h"
#include "library.h"

using namespace std;
void displayCommands()
{
    cout << 
}
int main()
{
    cout << "Welcome to <building name>" << endl;
    string command;
    string commands[4];
    Library lib;
    Details parking_lot;
    Board board;
    Flat flat_obj;
    while (true)
    {
        int index = 0;
        cout << "Enter command: ";
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
        transform(commands[0].begin(), commands[0].end(), commands[0].begin(), ::tolower);
        transform(commands[1].begin(), commands[1].end(), commands[1].begin(), ::tolower);
        if (commands[0] == "end" || commands[0] == "exit" || commands[0] == "quit")
        {
            cout << "Exiting program...\n";
            break;
        }
        else if (commands[0] + commands[1] == "enterlibrary")
        {
            lib.enterLibrary();
        }
        else if (commands[0] + commands[1] + commands[2] == "enterparkinglot")
        {
            parking_lot.enterParkingLot();
        }
        else if (commands[0] + commands[1] + commands[2] == "enterclubhouse")
        {
            cout << "The building is quite recently constructed and does not have full facilities yet\nCurrently we only have Othello\n";
            cout << "Would you like to play othello? (Y/n) ";
            char choice;
            while (1)
            {
                try
                {
                    cin >> choice;
                    break;
                }
                catch (...)
                {
                    cout << "Invalid choice\n";
                }
                if (choice == 'Y' || choice == 'y')
                {
                    cout << "Would you like to play with computer or with a friend? (C/f)";
                    try
                    {
                        cin >> choice;
                        if (choice == 'C' || choice == 'c')
                        {
                            OnePlayer obj(board);
                        }
                        else if (choice == 'f' || choice == 'F')
                        {
                            TwoPlayer(board);
                        }
                        else
                            throw(choice);
                        break;
                    }
                    catch (...)
                    {
                        cout << "Invalid choice\n";
                    }
                }
            }
        }
        else if (commands[0] + commands[1] == "buyflat")
        {
            try
            {
                flat_obj.bookFlat(stoi(commands[1]))
            }
            catch (...)
            {
                cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
            }
        }
        else if (commands[0] + commands[1] == "sellflat")
        {
            try
            {
                flat_obj.sellFlat(stoi(commands[1]));
            }
            catch (...)
            {
                cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
            }
        }
        else if (commands[0] == "showflat")
        {
            try
            {
                flat_obj.showFlat(stoi(commands[1]));
            }
            catch (...)
            {
                cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
            }
        }
        else if (commands[0] == "showavailableflats")
        {
            try
            {
                flat_obj.showAvailableFlats();
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
    }
    cout << "Program Exited\n";
    return 0;
}