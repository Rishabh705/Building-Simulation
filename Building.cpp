#include <iostream>
#include <algorithm>
#include "game.h"
#include "flat.h"
#include "parking.h"
#include "library.h"

using namespace std;
void displayCommands()
{
    cout << "Kindly enter one of the following commands to execute the corresponding task" << endl;
    cout << " 1. Enter library                    /* Enter library inside building */" << endl;
    cout << " 2. Enter parking lot                /* Enter parking lot beneath the building (basement) */" << endl;
    cout << " 3. Enter club house                 /* Enter club house to play available games */" << endl;
    cout << " 4. Buy flat <flat number>           /* Buy a flat */" << endl;
    cout << " 5. Sell flat <flat number>          /* Sell a flat */" << endl;
    cout << " 6. Show flat <flat number>          /* Show a specific flat */" << endl;
    cout << " 7. Show available flats             /* Show all available flats */" << endl;
    cout << " 8. clear                            /* clear terminal */" << endl;
    cout << " 9. help                             /* To display commands again */" << endl;
    cout << "10. exit                             /* To exit library */" << endl;
}

int main()
{
    cout << "Welcome to <building name>\nThis is a simulation\n" << endl;
    string command;
    string commands[4];
    Library lib;
    Details parking_lot;
    Flat flat_obj;
    displayCommands();
    while (true)
    {
        int index = 0;
        cout << "Enter command: ";
        getline(cin >> ws, command);
        transform(command.begin(), command.end(), command.begin(), ::tolower);
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
                        Board board;
                        if (choice == 'C' || choice == 'c')
                        {
                            OnePlayer obj(board);
                        }
                        else if (choice == 'f' || choice == 'F')
                        {
                            TwoPlayer obj(board);
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
                flat_obj.bookFlat(stoi(commands[2]));
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
                flat_obj.sellFlat(stoi(commands[2]));
            }
            catch (...)
            {
                cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
            }
        }
        else if (commands[0] + commands[1] == "showflat")
        {
            try
            {
                flat_obj.showFlat(stoi(commands[2]));
            }
            catch (...)
            {
                cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
            }
        }
        else if (commands[0] + commands[1] + commands[2] == "showavailableflats")
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