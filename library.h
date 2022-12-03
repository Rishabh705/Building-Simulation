#ifndef library_h
#define library_h
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ios>
#include <limits>
#include <unistd.h>
#include <dirent.h>

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
        cout << "days " << days << endl;
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
    vector<long long int> isbn_numbers;
    vector<int> reserved;
    vector<long long int> aadhar_numbers;
    vector<string> urls;
    int money;
    vector<Member> members;
    string command;
    string commands[4];
    void addBook(string bookname, long long int ISBN_number, int no_of_copies, string url) // add book to library (internal function only)
    {
        urls.insert(urls.end(), url);
        transform(bookname.begin(), bookname.end(), bookname.begin(), ::tolower);
        booknames.insert(booknames.end(), bookname);
        count.insert(count.end(), no_of_copies);
        reserved.insert(reserved.end(), 0);
        isbn_numbers.insert(isbn_numbers.end(), ISBN_number);
        return;
    }
    void addBook(string bookname, int no_of_copies, int id_number) // add book to library
    {
        if (id_numbers[id_number] != 1)
        {
            cout << "\nYou are not a manager and thus have no authority to add a book here" << endl;
            return;
        }
        long long int ISBN_number;
        if (no_of_copies > 0)
        {
            while (true)
            {
                cout << "\nEnter ISBN number: ";
                try
                {
                    cin >> ISBN_number;
                    if ((to_string(ISBN_number).length() == 13) || (to_string(ISBN_number).length() == 10))
                        break;
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
            for (int x = 0; x < isbn_numbers.size(); x++)
            {
                if (isbn_numbers[x] == ISBN_number)
                {
                    count[x] += no_of_copies;
                    cout << "Book(s) added to library successfully\n";
                    cout << "This terminal will be erased shortly\n";
                    sleep(5);
                    int system_return_val = system("cls");
                    if (system_return_val != 0)
                    {
                        system("clear");
                    }
                    return;
                }
            }
            cout << "If you have a url for an online copy of this book, please type/paste it here or else press enter: ";
            string url_temp;
            getline(cin >> ws, url_temp);
            string command_line;
            cout << "To check if this url works, this file will be opend now\nPress enter when ready to do so  ";

#ifdef _WIN64
            command_line = "start " + url_temp;
#elif __linux__
            command_line = "xdg-open " + url_temp;
#endif

            int system_return_val = system(command_line.c_str());
            if (system_return_val != 0)
            {
                cout << "\nThis url will not work so it wont be added\n";
                urls.insert(urls.end(), "");
            }
            else
            {
                cout << "This url works and hence it will be added\n";
                urls.insert(urls.end(), url_temp);
            }
            transform(bookname.begin(), bookname.end(), bookname.begin(), ::tolower);
            booknames.insert(booknames.end(), bookname);
            count.insert(count.end(), no_of_copies);
            reserved.insert(reserved.end(), 0);
            isbn_numbers.insert(isbn_numbers.end(), ISBN_number);
            cout << "Book(s) added to library successfully\n";
        }
        else
            cout << "\nKINDLY ENTER VALID NUMBER OF BOOKS!!!\nIT SHOULD BE WHOLE NUMBER!\n\n";
        cout << "This terminal will be erased shortly\n";
        sleep(5);
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
            cout << "\nBook Name\t\tISBN number\t\tNumber of copies available" << endl;
            for (int i = 0; i < booknames.size(); i++)
            {
                cout << booknames[i] << "\t\t\t" << isbn_numbers[i] << "\t\t\t\t" << count[i] << endl;
            }
            return;
        }
        cout << "\nLibrary is empty\n\n";
        return;
    }
    void addMember(int manager_id_number, string name, long long int member_aadhar_number) // add member to library
    {
        if (id_numbers[manager_id_number] != 1)
        {
            cout << "\nYou are not the manager and thus cannot add a member to the library\n";
            return;
        }
        if (to_string(member_aadhar_number).length() != 12)
        {
            cout << "\nInvalid aadhar number\n";
            return;
        }
        for (int x = 0; x < aadhar_numbers.size(); x++)
        {
            if (aadhar_numbers[x] == member_aadhar_number)
            {
                cout << "\nYou have already registered with this aadhar number\n";
                for (auto y = id_numbers.begin(); y != id_numbers.end(); y++)
                {
                    if (aadhar_numbers[y->second - 1] == member_aadhar_number)
                    {
                        cout << "Your id is " << y->first << endl;
                    }
                }
                return;
            }
        }
        int id_number = (id_numbers.size() * 100 + id_numbers.size() + 3) * 19; // A random way of generating id solely depending on number of users

        cout << "\nMember added successfully\n";
        cout << "\nHere is the id number for " << name << ": " << id_number << endl;
        aadhar_numbers.insert(aadhar_numbers.end(), member_aadhar_number);
        Member *member = new Member(name, id_number);
        members.insert(members.end(), *member);
        delete (member);
        mem++;
        id_numbers[id_number] = mem;
        cout << "KINDLY ASK MEMBER TO NOT FORGET THEIR ID NUMBER" << endl;
        sleep(3);
        while (true)
        {
            cout << "Have you told the member their id number? (Y/n) ";
            string c;
            cin >> c;
            if (c[0] == 'y' || c[0] == 'Y')
            {
                cout << "Good\n";
                cout << "The terminal will be cleared shortly\n";
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
            cout << "\nKINDLY ENTER VALID MEMBER ID NUMBER\n"
                 << endl;
            return;
        }
        member_id_number = id_numbers[member_id_number] - 1;
        if (members[member_id_number].amount_owed != 0)
        {
            cout << "\nYou (" << members[member_id_number].name << ") owe the library Rs" << members[member_id_number].amount_owed << endl
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
                        cout << "\nThe book is currently not available as all its copies have been issued\n\n";
                        cout << "Hence it will be reserved for you (" << members[member_id_number].name << ")\nYou ("
                             << members[member_id_number].name << ") will be providied with the book when it is available\n\n";
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
                        cout << "\nThe book '" << bookname << "'reserved by you (" << members[member_id_number].name << ") is now available and is currently being issued to '" << members[member_id_number].name << "' now\n\n";
                        members[member_id_number].reserved = 0;
                        reserved[i] = 0;
                    }
                    cout << "Book '" << bookname << "' issued successfully to '" << members[member_id_number].name << "'\n\n";
                    if (urls[i] != "")
                    {
                        while (true)
                        {
                            cout << "A soft copy for this book also exits if you want it (Y/n) ";
                            char choice;
                            cin >> choice;
                            string command_line;
                            if (choice == 'Y' || choice == 'y')
                            {
#ifdef _WIN64
                                command_line = "start " + urls[i];
#elif __linux__
                                command_line = "xdg-open " + urls[i];
#endif
                                cout << "Here you go then\n";
                                sleep(3);
                                system(command_line.c_str());
                                break;
                            }
                            else if (choice == 'n' || choice == 'N')
                                cout << "Okay\n";
                            else
                            {
                                cout << "Invalid choice\nTry again...\n";
                            }
                        }
                    }
                    return;
                }
            }
            cout << "No '" << bookname << "' book exists in the library\nThe library is relatively new" << endl
                 << "Kindly wait for some time and more books will arrive\n\n";
        }
        else
        {
            cout << "\n'" << bookname << "' will not be issued to you (" << members[member_id_number].name << ") as you ("
                 << members[member_id_number].name << ") have already have issued the book '" << members[member_id_number].bookname << "'\n\n";
        }
    }
    void issueBook(long long int ISBN_number, int member_id_number, int date)
    {
        if (id_numbers[member_id_number] == 0)
        {
            cout << "\nKINDLY ENTER VALID MEMBER ID NUMBER\n"
                 << members.size() << endl
                 << endl;
            return;
        }
        member_id_number = id_numbers[member_id_number] - 1;
        if (members[member_id_number].amount_owed != 0)
        {
            cout << "\nYou (" << members[member_id_number].name << ") owe the library Rs" << members[member_id_number].amount_owed << endl
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
                        cout << "\nThe book is currently not available as all its copies have been issued\n\n";
                        cout << "Hence it will be reserved for you (" << members[member_id_number].name << ")\nYou ("
                             << members[member_id_number].name << ") will be providied with the book when it is available\n\n";
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
                    if (urls[member_id_number] != "")
                    {
                        while (true)
                        {
                            cout << "A soft copy for this book also exits if you want it (Y/n) ";
                            char choice;
                            cin >> choice;
                            string command_line;
                            if (choice == 'Y' || choice == 'y')
                            {
#ifdef _WIN64
                                command_line = "start " + urls[i];
#elif __linux__
                                command_line = "xdg-open " + urls[i];
#endif
                                cout << "Here you go then\n";
                                sleep(3);
                                system(command_line.c_str());
                                break;
                            }
                            else if (choice == 'n' || choice == 'N')
                                cout << "Okay\n";
                            else
                            {
                                cout << "Invalid choice\nTry again...\n";
                            }
                        }
                    }
                    return;
                }
            }
            cout << "No book with ISBN number '" << ISBN_number << "' exists in the library\nThe library is relatively new\n"
                 << "Kindly wait for some time and more books will arrive\n\n";
        }
        else
        {
            cout << "\nBook with ISBN number '" << ISBN_number << "' will not be issued to you (" << members[member_id_number].name << ") as you (" << members[member_id_number].name << ") have already have issued the book '" << members[member_id_number].bookname << "'\n\n";
        }
    }
    void returnBook(int member_id_number, int date)
    {
        if (id_numbers[member_id_number] == 0)
        {
            cout << "\nKINDLY ENTER VALID MEMBER ID NUMBER\n"
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
                cout << "\nYou (" << members[member_id_number].name << ") owe the library fine of total Rs"
                     << members[member_id_number].amount_owed << "\n\n";
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
            cout << "\nYou (" << members[member_id_number].name << ") possess no book to return\n\n";
        }
    }
    void payDueAmount(int member_id_number)
    {
        if (id_numbers[member_id_number] == 0)
        {
            cout << "\nKINDLY ENTER VALID MEMBER ID NUMBER\n"
                 << members.size() << endl
                 << endl;
            return;
        }
        member_id_number = id_numbers[member_id_number] - 1;
        if (members[member_id_number].amount_owed == 0)
        {
            cout << "\nYou do not owe any money\n";
            return;
        }
        money += members[member_id_number].amount_owed;
        members[member_id_number].amount_owed = 0;
        cout << "Due paid successfully\n\n";
    }
    void displayCommands()
    {
        cout << "\nKindly enter one of the following commands to execute the corresponding task" << endl;
        cout << "1. Inventory                                                      /* To show books available in library */" << endl;
        cout << "2. addBook <book name> <number of copies> <your id (manager)>     /* To add a book in library (manager only)*/" << endl;
        cout << "3. addMember <name> <your id (manager)> <aadhar number>           /* To add member in library records (manager only)*/" << endl;
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
    }
    void exitLibrary()
    {
        FILE *fp;
        fp = fopen("library_inventory.csv", "a+"); // creates a file if it doesnt exist already
        fclose(fp);
        fstream file1("library_inventory.csv", fstream::out | fstream::trunc);
        file1 << "Inventory\n\n";
        file1 << "No., Books, ISBN number, Number of copies, url/path\n";
        for (int i = 0; i < booknames.size(); i++)
        {
            file1 << i + 1 << ", " + booknames[i] + ", " << isbn_numbers[i] << ", " << count[i] << ", " << urls[i] << "\n";
        }
        file1.close();
        fstream file2("library_members.csv", fstream::out | fstream::trunc);
        file2 << "MEMBER DATA\n\n";
        file2 << "Member name, Aadhar number, ID Number, Issued bookname, Reserved bookname, Amount owed, Issue date, Return date, Reserve date, Issued, Reserved\n";
        for (int i = 0; i < members.size(); i++)
        {
            file2 << members[i].name + ", " << aadhar_numbers[i] << ", " << members[i].id_number << ", " + members[i].bookname + ", " + members[i].reserved_bookname + ", "
                  << members[i].amount_owed << ", " << members[i].issue_date << ", " << members[i].return_date << ", " << members[i].reserve_date
                  << ", " << (bool)members[i].issued << ", " << (bool)members[i].reserved << "\n";
        }
        file2.close();
        cout << "\nExiting library ...\n";
        cout << "Library exited\n";
    }

public:
    void enterLibrary()
    {
        cout << "\nWelcome to the library\n"
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
                    cout << "\nINVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                }
            }
            else if (commands[0] == "addmember")
            {
                if (commands[1] == "")
                {
                    cout << "\nInvalid name\n\n";
                    continue;
                }
                try
                {
                    addMember(stoi(commands[2]), commands[1], stoll(commands[3]));
                }
                catch (...)
                {
                    cout << "\nINVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                }
            }
            else if (commands[0] == "issuebook")
            {
                try
                {
                    if (commands[3].length() == 6)
                        commands[3] = commands[3].substr(0, 4) + "20" + commands[3].substr(4);
                    long long int temp = stoll(commands[1]);
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
                        cout << "\nINVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                    }
                }
            }
            else if (commands[0] == "returnbook")
            {
                try
                {
                    if (commands[3].length() == 6)
                        commands[3] = commands[3].substr(0, 4) + "20" + commands[3].substr(4);
                    returnBook(stoi(commands[1]), stoi(commands[2]));
                }
                catch (...)
                {
                    cout << "\nINVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
                }
            }
            else if (commands[0] == "pay")
            {
                try
                {
                    payDueAmount(stoi(commands[1]));
                }
                catch (...)
                {
                    cout << "\nINVALID SYNTAX... Try again\nIf you are facing issues with commands, type 'help'\n";
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
                cout << "\nInvalid command... Try again\nIf you are facing issues with commands, type 'help'\n";
            }
        }
    }
    Library()
    {
        Member *member = new Member("Manager", 007);
        members.insert(members.end(), *member);
        id_numbers[007] = 1;
        aadhar_numbers.insert(aadhar_numbers.end(), 999999999999);
        delete (member);
        DIR *dr;
        struct dirent *en;
        dr = opendir("./books");
        if (dr)
        {
            for (int i = 1; (en = readdir(dr)) != NULL; i += 7)
            {
                string temp = en->d_name;
                if (temp.size() > 3)
                    if (temp.substr(temp.size() - 4) == ".pdf")
                        addBook(temp.substr(0, temp.size() - 4), 1111111111 + i + 100 * i + 10000 * i, 5, "./books/" + temp);
            }
        }
    }
};
#endif