#
<h1 align = "center">
Building Simulation
</h1>

`In this project we simulate a building using concepts of object-oriented programming with commands.`

`Inside the building, a customer is able to park his/her vehicle, view, checkin and checkout a flat, enjoy a game and read books in the library inside building.`

`The data of library, parking lot and flats is also copied to respective files of same name by the time program execution is completed`

This repository currently contains 6 files in which 5 are header files and 1 is driver file. Folders for sample [books](https://github.com/Rishabh705/Building-Simulation/tree/master/books) and [pictures](https://github.com/Rishabh705/Building-Simulation/tree/master/pics) are also uploaded.

You can view the UML for each of the classes used near the end of file description.

_Description of files are given below :_-

## 1. [flat.h](https://github.com/Rishabh705/Building-Simulation/blob/master/flat.h)

This file handles all operations related to Flats in building. It contains one class with members as follows :

``class Flat``  

Private Member Functions ->
+ void writeFile() : to write data of flats in a file.
+ void readFile() : to read data of library file.

Public Member Functions ->
+ Flats() : a default constructor to initialize flats.
+ void showAvailableFlats() : to display which flats are available for checkin.
+ void showFlats(int room_no) : to display interior image of the flat.
+ void bookFlat(int room_no) : takes an integer parameter to check if the room is available.
+ void sellFlat(int room_no) : to sell a flat when customer wants to leave the flat.

<p align = "center">
<img width = "400" src = "https://github.com/Rishabh705/Building-Simulation/blob/master/umls/Flat_UML.jpg" alt = "This image cannot be displayed as it is currently unavailable">
</p>


## 2. [parking.h](https://github.com/Rishabh705/Building-Simulation/blob/master/parking.h)

It handles all operations related to vehicle of customer. It contains two functions and three classes.

+ string lower(string str1) : to lowercase str1
+ int countWords(string line) : to count number of words in line.

``class Valid``

Protected Member Functions -> 
+ bool checkNumber(string reg) : to check validity of number plate.
+ bool checkColour(string clr) : to check validity of colour of vehicle

``class ParkingEntry``  inherits ``Valid``

Private Member Functions ->
+ writeFile() : to write data into a file 

Protected Member Functions -> 
+ bool exists(string reg) : to check if car is in parking lot
+ ParkingEntry() : to initialize empty_slots with blank 
+ void park(string reg, string clr, string pass) : to park car with number reg, colour clr, and pin of user.
+ void unpark(string reg, string pass) : to unpark car with number reg

``class Details``  inherits ``Parking Entry``

Private Member Functions -> 
+ void findVehicle(string reg) : to find vehicle in parking slot
+ void displayCommands() : to display list of commands
+ void inputCommands() : to input command

Public Member -> 
+ void enterParkingLot() : to use all above members 

<p align = "center">
<img width = "400" src = "https://github.com/Rishabh705/Building-Simulation/blob/master/umls/Parking_UML.jpg" alt = "This image cannot be displayed as it is currently unavailable">
</p>

## 3. [game.h](https://github.com/Rishabh705/Building-Simulation/blob/master/game.h)

This file provides an interactive game for customers. It contains 6 classes. Their brief description is given below : 

``class Board ``

Public Member Functions -> 
+ Board() : to initialize board

``class ValidMove``

Protected Member Functions ->
+ bool isSandwiched(int x, int y, int row_step, int col_step, char disk_colour, Board &t) : to check if a set of disks are flanked.
+ int checkDirections(int x, int y, char disk_colour, Board &t) : to check if a position is suitable for placing a disk.

``class Gameplay`` inherits ``ValidMove``

Protected Member Functions -> 
+ void flipDisks(int x, int y, int row_step, int col_step, char disk_colour, Board &t) : to flip flanked disks.
+ void changeCurrent(Board &t) : to change turn.
+ void placeDisk(int x, int y, char disk_colour, Board &t) : to place disks on board. 
+ void availableChoices(char disk_colour, Board &t) : to find all available moves. 
+ void min_maxFind(int row, int col, Board &t) : to check if a location gives max flips.
+ void min_maxFlipsLocation(char disk, Board &t) : to find position where there are maximum flips happening
+ void transfer(Board &t) : to transfer disks when a player is out of his disks 
+ bool finished(Board &t) : to check if game is over
+ int winner(Board &t) : to find the winner of the game

``class Display``

Public Member Functions ->
void PrintBoard(Board &t) : to display the board

``class TwoPlayer`` inherits ``Gameplay``

Public Member Functions ->
+ TwoPlayer(Board &t) : to handle game for two players

``class Computer`` inherits ``Gameplay``

Proteted Member Functions -> 
+ bool cornerAvailable(char disk, Board &t) : to check if corners are available for move
+ void play(char disk, Board &t) : to handle game for Oneplayer

``class OnePlayer`` inherits ``Computer``

Public Member Functions ->
+ OnePlayer(Board &t) : to handle game for One player

<p align = "center">
<img width = "1000" src = "https://github.com/Rishabh705/Building-Simulation/blob/master/umls/Othello_UML.jpg" alt = "This image cannot be displayed as it is currently unavailable">
</p>


## 4. [clubhouse.h](https://github.com/Rishabh705/Building-Simulation/blob/master/clubhouse.h)

This file handles all the games like othello game in [game.h](https://github.com/Rishabh705/Building-Simulation/blob/master/game.h) above. Here more class files similar to `game.h` for games can be added easily and thus no major changes will be needed.

This file has one class. It's breif description is given below:

`class ClubHouse`

Private Member Functions ->

+ void inputCommands(): To get commands input from user
+ void displayCommands(): To display commands to user
+ void enterClubHouse(): To start the functionality of club house ie play whichever game from the available ones the user wants to (this is created as a function and not a constructor so that we can enter and exit the club house as per our convenience without having to declare a new object everytime)

<p align = "center">
<img width = "400" src = "https://github.com/Rishabh705/Building-Simulation/blob/master/umls/Clubhouse_UML.jpg" alt = "This image cannot be displayed as it is currently unavailable">
</p>

## 5. [library.h](https://github.com/Rishabh705/Building-Simulation/blob/master/library.h)

This file handles Library management for the Building. It contains two classes. Their brief description is given below : 

``class Member``

Private Member Functions ->
+ int countLeapYears(int date) : to count total leapyears between return date and issue date.
+ int getDifference(int date1, int date2) : to get difference between to dates in days
+ void calculateAmountOwed() : to calculate fine

Public Member Functions ->
+ Member(string name, int id_number) : to initialize member details
 
``class Library``

Private Member Functions -> 
+ void addBook(string bookname, int no_of_copies, int id_number) : to add books in library
+  void addBook(string bookname, long long int ISBN_number, int no_of_copies, string url)
+ void showInventory() : to show books in library
+ void addMember(int manager_id_number, string name, long long int member_aadhar_number) : to register customer
+ void issueBook(string bookname, int member_id_number, int date) : to issue book using bookname.
+ void issueBook(int ISBN_number, int member_id_number, int date) : to issue book through ISBN number.
+ void returnBook(int member_id_number, int date) : to return issued book
+ void payDueAmount(int member_id_number) : to pay fine
+ void displayCommands() : to display all accepted commands
+ void inputCommands() : to take input from user
+ void exitLibrary() : to close library program

Public Member Functions ->
+ void enterLibrary() : to start library program
+ Library() : to initialize manager for library


<p align = "center">
<img width = "1000" src = "https://github.com/Rishabh705/Building-Simulation/blob/master/umls/Library_UML.jpg" alt = "This image cannot be displayed as it is currently unavailable">
</p>

## Driver code file : [Building.cpp](https://github.com/Rishabh705/Building-Simulation/blob/master/Building.cpp)

This file combines all above files and uses them to simulate a Building.

+ void displayCommands() : to display all accepted commands.
+ main()

 More files can be added easily and integrated with the program with near to no changes in original code. (only addition) 

`Some details:`
+ The [building](https://github.com/Rishabh705/Building-Simulation/blob/master/Building.cpp) contains 10 floors with 4 flats on each floor.
+ It also contains a [library](https://github.com/Rishabh705/Building-Simulation/blob/master/library.h), [parking lot](https://github.com/Rishabh705/Building-Simulation/blob/master/parking.h) and [clubhouse](https://github.com/Rishabh705/Building-Simulation/blob/master/clubhouse.h)
+ You can buy or sell flats
+ You can become a library member
+ You can park and unpark your car and find it
+ You can play [Othello](https://github.com/Rishabh705/Building-Simulation/blob/master/game.h) (currently only game in clubhouse)

`How to run the program?`

+ The user can start by running the [program](https://github.com/Rishabh705/Building-Simulation/blob/master/Building.cpp) and the possible instructions will be given to the user.
+ You can type 'help' whenever you forget the commands.
+ To issue a book in library, you need to be a member first. Manager id (to add user to library): 007
+ To park a car you need to provide a pin, so then only you can unpark the car.
+ Names with spaces will not be accepted.
+ You can check a sample run of the program [here](https://github.com/Rishabh705/Building-Simulation/blob/master/sample_input_output.mp4).

**[This repository](https://github.com/Rishabh705/Building-Simulation) is developed and owned by [Neel Amit Shah](https://github.com/Neelshah-01) and [Rishabh Singh](https://github.com/Rishabh705).**

*The pictures used were taken from [here](https://www.booking.com)*.