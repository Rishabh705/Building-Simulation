#ifndef CLUBHOUSE_H
#define CLUBHOUSE_H
#include <iostream>
#include <algorithm>
#include "game.h"
// You can add more Game libraries if you want to in here

using namespace std;

class ClubHouse
{
    string command;
    string commands[2];
    void inputCommands()
    {
        cout << "What would you like to do?\n";
        int index = 0;
        cout << "\nEnter command: ";
        getline(cin >> ws, command);
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        commands[0] = commands[1] = "";
        for (int i = 0; (i < command.length()) && (index < 2); i++)
        {
            if (command[i] == ' ')
                index++;
            else
                commands[index] += command[i];
        }
    }
    void displayCommands()
    {
        cout << "\nYour choices are as follows: \n";
        cout << "1. Play othello                  /* To play the game of othello */" << endl;
        cout << "2. Clear                         /* clear terminal */" << endl;
        cout << "3. Help                          /* Display commands */" << endl;
        cout << "4. Exit                          /* Exit club house */" << endl;
    }

public:
    void enterClubHouse()
    {
        cout << "Please note that the building is quite recently constructed and does not have full facilities yet\n\n";
        displayCommands();
        while (1)
        {
            inputCommands();
            if (commands[0] == "exit" || commands[0] == "end" || commands[0] == "quit")
            {
                cout << "Exiting club house...\n";
                break;
            }
            else if (commands[0] + commands[1] == "playothello")
            {
                char choice;
                while (1)
                {
                    cout << "Would you like to play with computer or with a friend? (C/f)";
                    cin >> choice;
                    Board board;
                    if (choice == 'C' || choice == 'c')
                    {
                        OnePlayer obj(board);
                        cout << "\nGame Over\nThank you for playing othello\n\n";
                    }
                    else if (choice == 'f' || choice == 'F')
                    {
                        TwoPlayer obj(board);
                        cout << "\nGame Over\nThank you for playing othello\n\n";
                    }
                    else
                        cout << "Invalid choice...\n";
                    break;
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
                cout << "Invalid command\n";
            }
        }
        cout << "Club house exited\n";
    }
};
#endif