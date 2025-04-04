#include <iostream>
#include <vector>
#include "Crawler.h"
#include "Board.h"
using namespace std;

void displayMenu() {
    cout << "Menu:\n";
    cout << "1. Initialize Bug Board (load data from file)\n";
    cout << "2. Display all Bugs\n";
    cout << "3. Find a Bug (given an id)\n";
    cout << "4. Tap the Bug Board (cause all to move, then fight/eat)\n";
    cout << "5. Display Life History of all Bugs (path taken)\n";
    cout << "6. Display all Cells listing their Bugs\n";
    cout << "7. Run simulation (generates a Tap every tenth of a second)\n";
    cout << "8. Exit (write Life History of all Bugs to file)\n";
    cout << "Enter your choice: ";
}

int main() {
    Board board(10, 10);
    int choice;
    bool exit = false;

    while (!exit) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                board.loadCrawlersFromFile("../crawler-bug.txt");
            cout << "Bug Board initialized.\n";
            break;
            case 2:
                board.displayAllBugs();
            break;
            case 3: {
                int id;
                cout << "Enter Bug ID: ";
                cin >> id;
                board.findBug(id);
                break;
            }
            case 4:
                board.tapBugBoard();
            board.displayAllBugs();
            break;
            case 5:
                board.displayLifeHistory();
            break;
            case 6:
                board.displayAllCells();
            break;
            case 7:
                board.simulateGame();
            break;
            case 8:
                board.writeLifeHistoryToFile();
            cout << "Life History written to file. Closingg...\n";
            exit = true;
            break;
            default:
                cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    return 0;
}