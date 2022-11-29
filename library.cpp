#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ios>
#include <limits>
#include <unistd.h>

using namespace std;
int mem = 1;
const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class Member
{
    string name = "NONE", bookname = "NONE", reserved_bookname = "NONE";
    int id_number = -1;
    int amount_owed = 0, issue_date = 0, return_date = 0, reserve_date = 0; // ddmmyyyy format only
    bool issued = 0, reserved = 0;                                          // no book, no reservation

    int countLeapYears(int date)
    {
        int years = date % 10000;
        int months = ((date - years) / 10000) % 100;
        int days = (date - years - (10000 * months)) / 1000000;

        if (months <= 2)
            years--;
        return years / 4 - years / 100 + years / 400;
    }

    int getDifference(int date1, int date2)
    {
        int years1 = date1 % 10000;
        int months1 = ((date1 - years1) / 10000) % 100;
        int days1 = (date1 - years1 - (10000 * months1)) / 1000000;

        int years2 = date2 % 10000;
        int months2 = ((date2 - years2) / 10000) % 100;
        int days2 = (date2 - years2 - (10000 * months2)) / 1000000;

        long int n1 = years1 * 365 + days1;
        for (int i = 0; i < months1 - 1; i++)
            n1 += monthDays[i];
        n1 += countLeapYears(date1);

        long int n2 = years2 * 365 + days2;
        for (int i = 0; i < months2 - 1; i++)
            n2 += monthDays[i];
        n2 += countLeapYears(date2);

        return (n2 - n1);
    }
    void calculateAmountOwed()
    {
        /* 5 rupees per day will be charged after 14 days */
        int days = getDifference(issue_date, return_date);
        if (days <= 14)
            return;
        amount_owed += (days - 14) * 5;
    }

public:
    Member(string name, int id_number)
    {
        this->name = name;
        this->id_number = id_number;
    }
    friend class Library;
};

class Library
{
    map<int, int> id_numbers;
    vector<string> booknames;
    vector<int> count;
    vector<int> isbn_numbers;
    vector<int> reserved;
    int money;
    vector<Member> members;
    string command;
    string commands[4];
    void addBook(string bookname, int no_of_copies, int id_number) // add book to library
    {
        if (id_numbers[id_number] != 1)
        {
            cout << "You are not a manager and thus have no authority to add a book here" << endl;
            return;
        }
        int ISBN_number;
        if (no_of_copies > 0)
        {
            while (true)
            {
                cout << "Enter ISBN number: ";
                try
                {
                    cin >> ISBN_number;
                    if (((ISBN_number < 10000000000000) && (ISBN_number > 999999999999)) || ((ISBN_number < 10000000000) && (ISBN_number > 999999999)))
                    {
                        break;
                    }
                    else
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw(invalid_argument("Enter 10 digit or 13 digit integer value without '-' character "));
                    }
                }
                catch (invalid_argument &e)
                {
                    cout << e.what() << endl;
                }
            }
            transform(bookname.begin(), bookname.end(), bookname.begin(), ::tolower);
            booknames.insert(booknames.end(), bookname);
            count.insert(count.end(), no_of_copies);
            reserved.insert(reserved.end(), 0);
            isbn_numbers.insert(isbn_numbers.end(), ISBN_number);
            cout << "Book added to library successfully\n";
        }
        else
            cout << "KINDLY ENTER VALID NUMBER OF BOOKS!!!\nIT SHOULD BE WHOLE NUMBER!\n\n";
        cout << "This terminal will be erased shortly\n";
        sleep(7);
        int system_return_val = system("cls");
        if (system_return_val != 0)
        {
            system("clear");
        }
        return;
    }
    void showInventory()
    {
        if (booknames.size() != 0)
        {
            cout << "Book Name\t\tNumber of copies available" << endl;
            for (int i = 0; i < booknames.size(); i++)
            {
                cout << "    " << booknames[i] << "\t\t\t\t" << count[i] << endl;
            }
            return;
        }
        cout << "Library is empty\n\n";
        return;
    }
    void addMember(int manager_id_number, string name) // add member to library
    {
        if (id_numbers[manager_id_number] != 1)
        {
            cout << "You are not the manager and thus cannot add a member to the library\n";
            return;
        }
        int id_number = (id_numbers.size() * 100 + id_numbers.size() + 3) * 19; // A random way of generating id solely depending on number of users

        cout << "Here is the id number for " << name << ": " << id_number << endl;
        Member *member = new Member(name, id_number);
        members.insert(members.end(), *member);
        delete (member);
        mem++;
        id_numbers[id_number] = mem;
        cout << "Kindly ask member to not forget their id number" << endl;
        cout << "Member added successfully\n";
        while (true)
        {
            cout << "Have you told the member their id number? (Y/n) ";
            string c;
            cin >> c;
            if (c[0] == 'y' || c[0] == 'Y')
            {
                cout << "Good\n";
                cout << "The terminal will be cleared shortly";
                sleep(5);
                int system_return_val = system("cls");
                if (system_return_val != 0)
                {
                    system("clear");
                }
                break;
            }
            cout << "Please inform the member\n";
            sleep(3);
        }
    }
    void issueBook(string bookname, int member_id_number, int date)
    {
        if (id_numbers[member_id_number] == 0)
        {
            cout << "KINDLY ENTER VALID MEMBER ID NUMBER\n"
                 << members.size() << endl
                 << endl;
            return;
        }
        member_id_number = id_numbers[member_id_number] - 1;
        if (members[member_id_number].amount_owed != 0)
        {
            cout << "You (" << members[member_id_number].name << ") owe the library Rs" << members[member_id_number].amount_owed << endl
                 << "No book is issued yet to you " << members[member_id_number].name << "\nKINDLY PAY Rs" << members[member_id_number].amount_owed << " FIRST\n\n";
            return;
        }
        if (members[member_id_number].issued == 0)
        {
            for (int i = 0; i < booknames.size(); i++)
            {
                if (booknames[i] == bookname)
                {
                    if (count[i] == 0)
                    {
                        cout << "The book is currently not available as all its copies have been issued\n\n";
                        cout << "Hence it will be reserved for you (" << members[member_id_number].name << ")\nYou (" << members[member_id_number].name << ") will be providied with the book when it is available\n\n";
                        members[member_id_number].reserve_date = date;
                        reserved[i] = 1;
                        members[member_id_number].reserved = 1;
                        members[member_id_number].reserved_bookname = bookname;
                        return;
                    }
                    count[i]--;
                    members[member_id_number].bookname = bookname;
                    members[member_id_number].issue_date = date;
                    members[member_id_number].return_date = 0;
                    members[member_id_number].issued = 1;
                    if ((members[member_id_number].reserved == 1) && (bookname == members[member_id_number].reserved_bookname))
                    {
                        cout << "The book '" << bookname << "'reserved by you (" << members[member_id_number].name << ") is now available and is currently being issued to '" << members[member_id_number].name << "' now\n\n";
                        members[member_id_number].reserved = 0;
                        reserved[i] = 0;
                    }
                    cout << "Book '" << bookname << "' issued successfully to '" << members[member_id_number].name << "'\n\n";
                    return;
                }
            }
            cout << "No '" << bookname << "' book exists in the library\nThe library is relatively new\nKindly wait for some time and more books will arrive\n\n";
        }
        else
        {
            cout << "'" << bookname << "' will not be issued to you (" << members[member_id_number].name << ") as you (" << members[member_id_number].name << ") have already have issued the book '" << members[member_id_number].bookname << "'\n\n";
        }
    }
    void issueBook(int ISBN_number, int member_id_number, int date)
    {
        if (id_numbers[member_id_number] == 0)
        {
            cout << "KINDLY ENTER VALID MEMBER ID NUMBER\n"
                 << members.size() << endl
                 << endl;
            return;
        }
        member_id_number = id_numbers[member_id_number] - 1;
        if (members[member_id_number].amount_owed != 0)
        {
            cout << "You (" << members[member_id_number].name << ") owe the library Rs" << members[member_id_number].amount_owed << endl
                 << "No book is issued yet to you " << members[member_id_number].name << "\nKINDLY PAY Rs" << members[member_id_number].amount_owed << " FIRST\n\n";
            return;
        }
        if (members[member_id_number].issued == 0)
        {
            for (int i = 0; i < booknames.size(); i++)
            {
                if (isbn_numbers[i] == ISBN_number)
                {
                    if (count[i] == 0)
                    {
                        cout << "The book is currently not available as all its copies have been issued\n\n";
                        cout << "Hence it will be reserved for you (" << members[member_id_number].name << ")\nYou (" << members[member_id_number].name << ") will be providied with the book when it is available\n\n";
                        members[member_id_number].reserve_date = date;
                        reserved[i] = 1;
                        members[member_id_number].reserved = 1;
                        members[member_id_number].reserved_bookname = booknames[i];
                        return;
                    }
                    count[i]--;
                    members[member_id_number].bookname = booknames[i];
                    members[member_id_number].issue_date = date;
                    members[member_id_number].return_date = 0;
                    members[member_id_number].issued = 1;
                    if ((members[member_id_number].reserved == 1) && (booknames[i] == members[member_id_number].reserved_bookname))
                    {
                        cout << "The book '" << booknames[i] << "'reserved by you (" << members[member_id_number].name << ") is now available and is currently being issued to '" << members[member_id_number].name << "' now\n\n";
                        members[member_id_number].reserved = 0;
                        reserved[i] = 0;
                    }
                    cout << "Book '" << booknames[i] << "' issued successfully to '" << members[member_id_number].name << "'\n\n";
                    return;
                }
            }
            cout << "No book with ISBN number '" << ISBN_number << "' exists in the library\nThe library is relatively new\nKindly wait for some time and more books will arrive\n\n";
        }
        else
        {
            cout << "Book with ISBN number '" << ISBN_number << "' will not be issued to you (" << members[member_id_number].name << ") as you (" << members[member_id_number].name << ") have already have issued the book '" << members[member_id_number].bookname << "'\n\n";
        }
    }
    void returnBook(int member_id_number, int date)
    {
        if (id_numbers[member_id_number] == 0)
        {
            cout << "KINDLY ENTER VALID MEMBER ID NUMBER\n"
                 << members.size() << endl
                 << endl;
            return;
        }
        member_id_number = id_numbers[member_id_number] - 1;
        if (members[member_id_number].issued == 1)
        {
            int book_index = -1;
            string bookname;
            for (int i = 0; i < booknames.size(); i++)
            {
                if (booknames[i] == members[member_id_number].bookname)
                {
                    bookname = members[member_id_number].bookname;
                    members[member_id_number].bookname = "NONE";
                    count[i]++;
                    members[member_id_number].return_date = date;
                    members[member_id_number].issued = 0;
                    cout << "The book '" << booknames[i] << "' is returned successfully\n\n";
                    if (members[member_id_number].reserved == 1)
                    {
                        for (int i = 0; i < booknames.size(); i++)
                        {
                            if (booknames[i] == members[member_id_number].reserved_bookname)
                            {
                                if (count[i] != 0)
                                {
                                    cout << "The book '" << members[member_id_number].bookname << "'reserved by you (" << members[member_id_number].name << ") is now available and is currently being issued to you (" << members[member_id_number].name << ") now\n\n";
                                    members[member_id_number].reserved = 0;
                                    reserved[i] = 0;
                                }
                            }
                        }
                    }
                    members[member_id_number].calculateAmountOwed();
                    book_index = i;
                }
            }
            if (members[member_id_number].amount_owed != 0)
            {
                cout << "You (" << members[member_id_number].name << ") owe the library fine of total Rs" << members[member_id_number].amount_owed << endl
                     << endl;
            }
            if (reserved[book_index] != 0)
            {
                int priority_date = 99999999, priority_index = -1;
                for (int j = 0; j < members.size(); j++)
                {
                    if (members[j].bookname == booknames[book_index])
                    {
                        if ((members[j].getDifference(priority_date, members[j].reserve_date) > 0) && (members[j].issued != 0))
                        {
                            priority_date = members[j].reserve_date;
                            priority_index = j;
                        }
                    }
                }
                issueBook(bookname, priority_index, date);
                reserved[book_index]--;
            }
        }
        else
        {
            cout << "You (" << members[member_id_number].name << ") possess no book to return\n\n";
        }
    }
    void payDueAmount(int member_id_number)
    {
        if (id_numbers[member_id_number] == 0)
        {
            cout << "KINDLY ENTER VALID MEMBER ID NUMBER\n"
                 << members.size() << endl
                 << endl;
            return;
        }
        member_id_number = id_numbers[member_id_number] - 1;
        money += members[member_id_number].amount_owed;
        members[member_id_number].amount_owed = 0;
        cout << "Due paid successfully\n\n";
    }
    void displayCommands()
    {
        cout << "Kindly enter one of the following commands to execute the corresponding task" << endl;
        cout << "1. Inventory                                                      /* To show books available in library */" << endl;
        cout << "2. addBook <book name> <number of copies> <your id (manager)>     /* To add a book in library (manager only)*/" << endl;
        cout << "3. addMember <name> <your id (manager)>                           /* To add member in library records (manager only)*/" << endl;
        cout << "4. issueBook <book name/isbn number> <your id> <date(ddmmyyyy)>   /* To issue book to member */" << endl;
        cout << "5. returnBook <your id> <date(ddmmyyyy)>                          /* To return book to library */" << endl;
        cout << "6. pay <your id>                                                  /* To pay amount owed to library */" << endl;
        cout << "7. clear                                                          /* clear terminal */" << endl;
        cout << "8. help                                                           /* To display commands again */" << endl;
        cout << "9. exit                                                           /* To exit library */" << endl;
    }
    void inputCommands()
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
        cout << "commands[0]: " << commands[0] << endl;
        cout << "commands[1]: " << commands[1] << endl;
        cout << "commands[2]: " << commands[2] << endl;
        cout << "commands[3]: " << commands[3] << endl;
    }
    void exitLibrary()
    {
        FILE *fp;
        fp = fopen("library.txt", "a+"); // creates a file if it doesnt exist already
        fclose(fp);
        fstream file("library.txt", fstream::out | fstream::trunc);
        file << "Inventory\n";
        file << "No.\t\tBooks\t\tNumber of copies\n";
        for (int i = 0; i < booknames.size(); i++)
        {
            file << i + 1 << "\t" + booknames[i] + "\t\t" << count[i] << "\n";
        }
        file << "\n\nMEMBER DATA\n";
        file << "Member name \t ID Number\t Issued bookname \t Reserved bookname \t Amount owed \t Issue date \t Return date \t Reserve date \t Issued \t Reserved\n";
        for (int i = 0; i < members.size(); i++)
        {
            file << members[i].name + " \t\t " << members[i].id_number << "  \t\t " + members[i].bookname + " \t\t\t\t " + members[i].reserved_bookname + " \t\t\t\t "
                 << members[i].amount_owed << " \t\t\t\t " << members[i].issue_date << " \t\t\t\t " << members[i].return_date << " \t\t\t\t " << members[i].reserve_date
                 << " \t\t\t\t " << (bool)members[i].issued << " \t\t\t " << (bool)members[i].reserved << "\n";
        }
        file.close();
        cout << "Exiting library ...\n";
        cout << "Library exited\n";
    }

public:
    void enterLibrary()
    {
        cout << "Welcome to the library\n"
             << endl;
        displayCommands();
        while (true)
        {
            inputCommands();
            transform(commands[0].begin(), commands[0].end(), commands[0].begin(), ::tolower);
            transform(commands[1].begin(), commands[1].end(), commands[1].begin(), ::tolower);
            if (commands[0] == "end" || commands[0] == "exit" || commands[0] == "quit")
            {
                exitLibrary();
                break;
            }
            else if (commands[0] == "inventory")
            {
                showInventory();
            }
            else if (commands[0] == "addbook")
            {
                try
                {
                    addBook(commands[1], stoi(commands[2]), stoi(commands[3]));
                }
                catch (...)
                {
                    cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                }
            }
            else if (commands[0] == "addmember")
            {
                if (commands[1] == "")
                {
                    cout << "Invalid name\n\n";
                    continue;
                }
                try
                {
                    addMember(stoi(commands[2]), commands[1]);
                }
                catch (...)
                {
                    cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                }
            }
            else if (commands[0] == "issuebook")
            {
                try
                {
                    int temp = stoi(commands[1]);
                    issueBook(temp, stoi(commands[2]), stoi(commands[3]));
                }
                catch (...)
                {
                    try
                    {
                        issueBook(commands[1], stoi(commands[2]), stoi(commands[3]));
                    }
                    catch (...)
                    {
                        cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                    }
                }
            }
            else if (commands[0] == "reutrnbook")
            {
                try
                {
                    returnBook(stoi(commands[1]), stoi(commands[2]));
                }
                catch (...)
                {
                    cout << "INVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                }
            }
            else if (commands[0] == "pay")
            {
                try
                {
                    payDueAmount(stoi(commands[1]));
                }
                catch(...)
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
                cout << "Invalid command... Try again\nIf you are facing issues with commands, type 'help'\n";
            }
        }
    }
    Library()
    {
        Member *member = new Member("Manager", 007);
        members.insert(members.end(), *member);
        id_numbers[007] = 1;
        delete (member);
    }
};