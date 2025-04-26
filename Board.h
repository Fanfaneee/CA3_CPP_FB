#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"
#include "Jumper.h"

class Board {
private:
    int width;
    int height;
    std::vector<Bug*> bugs;
    std::vector<Bug*> deadBugs;
    std::map<Position, std::vector<Bug*>> cellOccupants;

    void updateCellOccupants();

public:
    Board(int w, int h);
    ~Board();

    void addBug(Bug* bug);
    void loadCrawlersFromFile(const std::string& filename);
    void moveBugs();
    void displayAllBugs() const;
    void findBug(int id) const;
    bool tapBugBoard();
    void fight();
    void displayLifeHistory() const;
    void writeLifeHistoryToFile() const;
    void displayAllCells() const;
    bool checkLastBugStanding() const;
    void simulateGame();
};

#endif // BOARD_H