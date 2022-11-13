#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <bits/stdc++.h>
#define MAX 50
using namespace std;
class Board
{
protected:
    vector<vector<char>> board; // 8x8
    int current_player, left_white_disks, left_black_disks, white_on_board, black_on_board, total_disks_on_board, both_cant_makemove, other_clr_disk, min_flip, max_flip, min_row, min_col, max_row, max_col;
    char current_player_disk = 'B', player1_disk = 'B', player2_disk = 'W';
    vector<vector<int>> avail;
    friend class Display;
    friend class ValidMove;
    friend class Gameplay;
    friend class TwoPlayer;
    friend class OnePlayer;
    friend class Computer;

public:
    Board()
    {
        current_player = min_flip = max_flip = 1;
        total_disks_on_board = both_cant_makemove = other_clr_disk = min_col = min_row = max_col = max_row = 0;
        for (int i = 0; i < 8; i++)
            board.push_back(vector<char>(8, ' '));
        board[3][3] = 'W'; // white -> o  black -> x
        board[4][4] = 'W';
        board[4][3] = 'B';
        board[3][4] = 'B';
        left_white_disks = 30;
        left_black_disks = 30;
        white_on_board = 2;
        black_on_board = 2;
    }
};
class ValidMove
{
protected:
    bool isSandwiched(int x, int y, int row_step, int col_step, char disk_colour, Board &t)
    {
        t.other_clr_disk = 0;
        char other_clr;
        if (disk_colour == 'B')
            other_clr = 'W';
        else
            other_clr = 'B';
        if ((x < 0) || (x >= 8) || (y < 0) || (y >= 8))
            return false;
        if (t.board[x][y] == other_clr)
        {
            while ((x >= 0) && (x < 8) && (y >= 0) && (y < 8))
            {
                if (x > -1 && y > -1)
                {
                    if (t.board[x][y] == ' ')
                        return false;
                    if (t.board[x][y] == disk_colour)
                        return true;
                    t.other_clr_disk++;
                }
                x += row_step;
                y += col_step;
            }
        }
        return false;
    }
    int checkDirections(int x, int y, char disk_colour, Board &t)
    {
        if (isSandwiched(x - 1, y, -1, 0, disk_colour, t)) // up
            return 1;
        if (isSandwiched(x + 1, y, 1, 0, disk_colour, t)) // down
            return 2;
        if (isSandwiched(x, y - 1, 0, -1, disk_colour, t)) // left
            return 3;
        if (isSandwiched(x, y + 1, 0, 1, disk_colour, t)) // right
            return 4;
        if (isSandwiched(x + 1, y + 1, 1, 1, disk_colour, t)) // down right
            return 5;
        if (isSandwiched(x + 1, y - 1, 1, -1, disk_colour, t)) // down left
            return 6;
        if (isSandwiched(x - 1, y + 1, -1, 1, disk_colour, t)) // up right
            return 7;
        if (isSandwiched(x - 1, y - 1, -1, -1, disk_colour, t)) // up left
            return 8;
        return 0;
    }
};
class Gameplay : public ValidMove
{
protected:
    void flipDisks(int x, int y, int row_step, int col_step, char disk_colour, Board &t)
    {
        char other_clr = 'W';
        if (disk_colour == 'W')
            other_clr = 'B';
        while (t.board[x][y] == other_clr)
        {
            t.board[x][y] = disk_colour;
            if (disk_colour == 'W')
            {
                t.white_on_board++;
                t.black_on_board--;
            }
            else
            {
                t.black_on_board++;
                t.white_on_board--;
            }
            x += row_step;
            y += col_step;
        }
    }
    void changeCurrent(Board &t)
    {
        if (t.current_player == 1)
        {
            t.current_player = 2;
            t.current_player_disk = t.player2_disk;
        }
        else
        {
            t.current_player = 1;
            t.current_player_disk = t.player1_disk;
        }
    }
    void placeDisk(int x, int y, char disk_colour, Board &t)
    {
        char other_clr = 'W';
        if (disk_colour == 'W')
            other_clr = 'B';
        if (t.board[x][y] == ' ')
        {
            t.board[x][y] = disk_colour;
            if (disk_colour == 'B')
                t.black_on_board++;
            else
                t.white_on_board++;
            if (checkDirections(x, y, disk_colour, t) == 1)
                flipDisks(x - 1, y, -1, 0, disk_colour, t);
            if (checkDirections(x, y, disk_colour, t) == 2)
                flipDisks(x + 1, y, 1, 0, disk_colour, t);
            if (checkDirections(x, y, disk_colour, t) == 3)
                flipDisks(x, y - 1, 0, -1, disk_colour, t);
            if (checkDirections(x, y, disk_colour, t) == 4)
                flipDisks(x, y + 1, 0, 1, disk_colour, t);
            if (checkDirections(x, y, disk_colour, t) == 5)
                flipDisks(x + 1, y + 1, 1, 1, disk_colour, t);
            if (checkDirections(x, y, disk_colour, t) == 6)
                flipDisks(x + 1, y - 1, 1, -1, disk_colour, t);
            if (checkDirections(x, y, disk_colour, t) == 7)
                flipDisks(x - 1, y + 1, -1, 1, disk_colour, t);
            if (checkDirections(x, y, disk_colour, t) == 8)
                flipDisks(x - 1, y - 1, -1, -1, disk_colour, t);
            changeCurrent(t);
            t.left_black_disks = 32 - t.black_on_board;
            t.left_white_disks = 32 - t.white_on_board;
        }
        else
            cout << "Move Not valid\n";
    }
    void availableChoices(char disk_colour, Board &t)
    {
        t.avail.clear();
        int x = 0;
        for (size_t i = 0; i < 8; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                if (t.board[i][j] == ' ' && checkDirections(i, j, disk_colour, t))
                {
                    t.avail.push_back(vector<int>(2, 0));
                    t.avail[x][0] = i;
                    t.avail[x][1] = j;
                    x++;
                }
            }
        }
    }
    void min_maxFind(int row, int col, Board &t)
    {
        if (t.max_flip <= t.other_clr_disk)
        {
            t.max_flip = t.other_clr_disk;
            t.max_row = row;
            t.max_col = col;
        }
        if (t.min_flip >= t.other_clr_disk)
        {
            t.min_flip = t.other_clr_disk;
            t.min_row = row;
            t.min_col = col;
        }
    }
    void min_maxFlipsLocation(char disk, Board &t)
    {
        availableChoices(disk, t);
        int row, col;
        for (int i = 0; i < t.avail.size(); i++)
        {
            t.min_flip = t.max_flip = 0;
            row = t.avail[i][0];
            col = t.avail[i][1];
            isSandwiched(row - 1, col, -1, 0, disk, t);
            min_maxFind(row, col, t);
            isSandwiched(row + 1, col, 1, 0, disk, t);
            min_maxFind(row, col, t);
            isSandwiched(row, col - 1, 0, -1, disk, t);
            min_maxFind(row, col, t);
            isSandwiched(row, col + 1, 0, 1, disk, t);
            min_maxFind(row, col, t);
            isSandwiched(row + 1, col + 1, 1, 1, disk, t);
            min_maxFind(row, col, t);
            isSandwiched(row + 1, col - 1, 1, -1, disk, t);
            min_maxFind(row, col, t);
            isSandwiched(row - 1, col + 1, -1, 1, disk, t);
            min_maxFind(row, col, t);
            isSandwiched(row - 1, col - 1, -1, -1, disk, t);
            min_maxFind(row, col, t);
        }
    }
    void transfer(Board &t)
    {
        t.total_disks_on_board = t.black_on_board + t.white_on_board;
        if (t.left_white_disks <= 0 && t.total_disks_on_board < 64)
        {
            t.left_white_disks++;
            t.left_black_disks--;
        }
        else if (t.left_black_disks <= 0 && t.total_disks_on_board < 64)
        {
            t.left_black_disks++;
            t.left_white_disks--;
        }
    }
    bool finished(Board &t)
    {
        for (size_t i = 0; i < 8; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                if (t.board[i][j] == ' ')
                    return false;
            }
        }
        return true;
    }
    int winner(Board &t)
    {
        int winner;
        if (t.black_on_board > t.white_on_board)
            return 1;
        else if (t.black_on_board < t.white_on_board)
            return 2;
        else
            return 0;
    }
};
class Display
{
public:
    void printBoard(Board &t)
    {
        cout << "    0    1    2    3    4    5    6    7\n";
        cout << " +----+----+----+----+----+----+----+----+\n";
        for (int i = 0; i < 8; i++)
        {
            cout << i << "|";
            for (int j = 0; j < 8; j++)
            {
                if (t.board[i][j] == ' ')
                    cout << t.board[i][j] << "   |";
                else
                    cout << " " << t.board[i][j] << "  |";
            }
            cout << "\n +----+----+----+----+----+----+----+----+\n";
        }
        cout << "Black disks : " << t.black_on_board << "\t\tWhite disks : " << t.white_on_board << endl
             << endl;
    }
};
class TwoPlayer : public Gameplay
{
public:
    TwoPlayer(Board &t)
    {
        Display obj;
        obj.printBoard(t);
        t.current_player = 1;
        t.current_player_disk = 'B';
        while (!finished(t) && t.both_cant_makemove != 2)
        {
            int res;
            availableChoices(t.current_player_disk, t);
            if (t.avail.size() > 0)
            {
                cout << "Turn of Player" << t.current_player << endl;
                transfer(t);
                t.both_cant_makemove = 0;
                cout << "Available choices are : \n";
                for (size_t i = 0; i < t.avail.size(); i++)
                    cout << i + 1 << ". (" << t.avail[i][0] << "," << t.avail[i][1] << ")" << endl;
                cout << "Enter the position : ";
                cin >> res;
                if (res <= t.avail.size())
                    placeDisk(t.avail[res - 1][0], t.avail[res - 1][1], t.current_player_disk, t);
                else
                    cout << "Enter from Available Choices!\n";
                obj.printBoard(t);
            }
            else
            {
                t.both_cant_makemove++;
                changeCurrent(t);
            }
        }
        if (winner(t) != 0)
            cout << "Winner of the game is : Player" << winner(t) << endl;
        else
            cout << "Its a tie!\n";
    }
};
class Computer : public Gameplay
{
protected:
    int row, col;
    bool cornerAvailable(char disk, Board &t)
    {
        for (size_t i = 0; i < t.avail.size(); i++)
        {
            if ((t.avail[i][0] == 0 && (t.avail[i][1] == 0 | t.avail[i][1] == 7)) | t.avail[i][0] == 7 && (t.avail[i][1] == 0 | t.avail[i][1] == 7))
            {
                row = t.avail[i][0];
                col = t.avail[i][1];
                return true;
            }
        }
        return false;
    }
    void play(char disk, Board &t)
    {
        min_maxFlipsLocation(disk, t);
        if (t.total_disks_on_board < 32) // do min flips
        {
            row = t.min_row;
            col = t.min_col;
        }
        else // do max flips
        {
            row = t.max_row;
            col = t.max_col;
        }
    }
};
class OnePlayer : public Computer
{
public:
    OnePlayer(Board &h)
    {
        Display obj;
        obj.printBoard(h);
        h.current_player = 1;
        h.current_player_disk = 'B';
        while (!finished(h) && h.both_cant_makemove < 2)
        {
            availableChoices(h.current_player_disk, h);
            if (h.avail.size() > 0)
            {
                transfer(h);
                if (h.current_player == 1)
                {
                    cout << "Turn of Player" << endl;
                    int res;
                    h.both_cant_makemove = 0;
                    cout << "Available choices are : \n";
                    for (size_t i = 0; i < h.avail.size(); i++)
                    {
                        cout << i + 1 << ". (" << h.avail[i][0] << "," << h.avail[i][1] << ")" << endl;
                    }
                    cout << "Enter the position : ";
                    cin >> res;
                    if (res <= h.avail.size())
                        placeDisk(h.avail[res - 1][0], h.avail[res - 1][1], h.current_player_disk, h);
                    else
                        cout << "Enter from Available Choices!\n";
                }
                else if (h.current_player == 2)
                {
                    h.both_cant_makemove = 0;
                    cout << "Turn of Computer" << endl;
                    if (!cornerAvailable(h.current_player_disk, h))
                    {
                        play(h.current_player_disk, h);
                    }
                    placeDisk(row, col, h.current_player_disk, h);
                }
            }
            else
            {
                h.both_cant_makemove++;
                changeCurrent(h);
            }
            obj.printBoard(h);
        }
        if (winner(h) == 1)
            cout << "Winner of the game is : Player" << endl;
        else if (winner(h) == 2)
            cout << "Winner of the game is : Computer" << endl;
        else
            cout << "Its a tie!\n";
    }
};
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
        string stateCodes[] = {"AN", "AP", "AR", "AS", "BH", "BR", "CH", "CG", "DD", "DL, GA",
                               "GJ", "HR", "HP", "JK", "JH", "KA", "KL", "LA", "LD", "MP", "MH",
                               "MN", "ML", "MZ", "NL", "OD", "PY", "PB", "RJ", "SK", "TN", "TS",
                               "TR", "UP", "UK", "WB"};
        if (reg.length() == 9)
        {
            if (isdigit(reg[0]) && isdigit(reg[1]) && isdigit(reg[4]) && isalpha(reg[2]) && isalpha(reg[3]) && isalpha(reg[5]) && isalpha(reg[6]) && isalpha(reg[7]) && isalpha(reg[8]))
                return false;
            for (size_t i = 0; i < 37; i++)
                if (((lower(stateCodes[i]).compare(lower(reg.substr(3)))) == 0))
                    return true;
            return false;
        }
        else if (reg.length() == 10)
        {
            if (isdigit(reg[0]) && isdigit(reg[1]) && isdigit(reg[4]) && isdigit(reg[5]) && isalpha(reg[2]) && isalpha(reg[3]) && isalpha(reg[6]) && isalpha(reg[7]) && isalpha(reg[8]) && isalpha(reg[9]))
                return false;
            for (size_t i = 0; i < 37; i++)
                if (((lower(stateCodes[i]).compare(lower(reg.substr(3)))) == 0))
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

protected:
    vector<vector<string>> slots;

public:
    ParkingEntry()
    {
        for (size_t i = 0; i < MAX; i++)
            empty_slots.push_back(i); // as initially all slots are empty.
        for (int i = 0; i < 4; i++)
            slots.push_back(vector<string>()); // Add an empty row
    }
    bool exists(string reg)
    {
        for (size_t i = 0; i < slots[1].size(); i++)
        {
            if (reg.compare(slots[1][i]) == 0)
                return true;
        }
        return false;
    }
    void park(string n, string clr, string pass) // park car of particular color = clr  and number = n at nearest slot.
    {
        if (checkNumber(n) && checkColour(clr))
        {
            if (!exists(n))
            {
                if (j < MAX)
                {
                    int nearest = *min_element(empty_slots.begin(), empty_slots.end()); // nearest available slot obtained
                    slots[0].push_back(clr);
                    slots[1].push_back(n);
                    slots[2].push_back(to_string(nearest));
                    slots[3].push_back(pass);
                    auto it = find(empty_slots.begin(), empty_slots.end(), nearest);
                    int index = it - empty_slots.begin();
                    empty_slots.erase(empty_slots.begin() + index);
                    cout << "Your car " << n << " is parked at slot number " << nearest + 1 << endl;
                    j++;
                }
                else
                    cout << "Sorry...Parking Lot is full!\n";
            }
            else
            {
                cout << "Car " << n << " already parked.\n";
                cout << "DAILING 100...\nALERT! All units...Location of suspected is tranferred...\nInitiating Lockdown...\nALERT! All units in area are ordered to engage...Shoot at sight...\n";
            }
        }
        else
            cout << "Incorrect data!!\n";
    }
    void unpark(string n, string pass)
    {
        bool car_was_present = false;
        int i;
        for (i = 0; i < j; i++)
        {
            if (n.compare(slots[1][i]) == 0 && pass.compare(slots[3][i]) == 0)
            {
                slots[0].erase(slots[0].begin() + i);
                slots[1].erase(slots[1].begin() + i);
                slots[2].erase(slots[2].begin() + i);
                slots[3].erase(slots[3].begin() + i);
                cout << "Your car " << n << " is unparked!\n";
                car_was_present = true;
                j--;
                break;
            }
        }
        if (car_was_present)
            empty_slots.push_back(i);
        else
            cout << "Oops!..No such car present.\n";
    }
};
class Details : public ParkingEntry
{
public:
    void findCar(string reg, int slot_number, bool flag)
    {
        bool found = false;
        for (size_t i = 0; i < j; i++)
        {
            if (flag == true)
            {
                if (reg.compare(slots[1][i]) == 0)
                {
                    cout << "Car number " << reg << " : " << endl;
                    cout << "Colour of " << slots[1][i] << " is : " << slots[0][i] << endl;
                    cout << "Slot number of " << slots[1][i] << " is : " << stoi(slots[2][i]) + 1 << endl;
                    found = true;
                    break;
                }
            }
            else
            {
                if (to_string(slot_number - 1).compare(slots[2][i]) == 0)
                {
                    cout << "Slot " << slot_number << " : " << endl;
                    cout << "Car number is : " << slots[1][i] << endl;
                    cout << "Colour of " << slots[1][i] << " is : " << slots[0][i] << endl;
                    found = true;
                    break;
                }
            }
        }
        if (found == false)
            cout << "Oops!..No data found.\n";
    }
    void findByClr(string clr)
    {
        bool found = false;
        bool check = checkColour(clr);
        if (check)
        {
            cout << "Registration number\t\tSlot\n";
            for (size_t i = 0; i < j; i++)
            {
                if (clr.compare(slots[0][i]) == 0)
                {
                    cout << "   " << slots[1][i] << "\t\t\t  " << stoi(slots[2][i]) + 1 << "\n";
                    found = true;
                }
            }
        }
        if (found == false)
            cout << "Oops!..No data found.\n";
    }
};

class GODNRAZOR : public Details
{
    void reception(string reg, string clr, string pass)
    {
        park(reg, clr, pass);
    }
};

class Customer : public TwoPlayer, public Computer, public OnePlayer
{
    int NR_coins;
    int intelligence;

public:
    void buyNR_coins()
    {
    }
    void playGame()
    {
        int res;
        do
        {
            cout << "\t\t\t---OTHELLO---\n\nMenu:\n1.Two Player\n2.One Player\n3.Exit\nEnter Your Choice : ";
            cin >> res;
            Board t;
            if (res == 1)
                TwoPlayer obj(t);
            else if (res == 2)
                OnePlayer obj(t);
            else if (res == 3)
            {
                cout << "Thank you\n";
                break;
            }
            else
                cout << "\nInvalid Choice\n";
        } while (true);
    }
};
int main()
{
    string line, word, reg, clr, nme;
    stringstream iss(line);
    ParkingEntry obj;
    Details ob;
    while (1)
    {
        cout << "\nEnter command or type 'quit' to exit\n";
        getline(cin, line);
        stringstream iss(line);
        iss >> word;
        if (word.compare("park") == 0 && countWords(line) == 3)
        {
            iss >> reg;
            iss >> clr;
            cout << "Enter your Password : ";
            cin >> nme;
            obj.park(reg, clr, nme);
        }
        else if (word.compare("unpark") == 0)
        {
            iss >> reg;
            cout << "Enter your Password : ";
            cin >> nme;
            obj.unpark(reg, nme);
        }
        else if (word.compare("find_parking_slot") == 0)
        {
            iss >> reg;
            ob.findCar("", stoi(reg), false);
        }
        else if (word.compare("find_car_number") == 0)
        {
            iss >> reg;
            ob.findCar(reg, 0, true);
        }
        else if (word.compare("find_car") == 0)
        {
            iss >> clr;
            ob.findByClr(clr);
        }
        else if (lower(word).compare("quit") == 0)
        {
            cout << "Exiting...\n";
            break;
        }
        else
            cout << "Error : Inappropriate command\n";
    }

    return 0;
}