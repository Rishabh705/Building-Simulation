#ifndef GAME_H
#define GAME_H
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
#endif