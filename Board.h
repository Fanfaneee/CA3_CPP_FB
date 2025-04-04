#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>
#include "Crawler.h"
using namespace std;

class Board {
private:
    int width;
    int height;
    vector<Crawler*> crawlers;
    vector<Crawler*> deadCrawlers;
    map<Position, vector<Crawler*>> cellOccupants;

    void majCellOccupants();

public:
    Board(int w, int h);

    void addCrawler(int id, int x, int y, Direction dir, int size);
    void moveCrawlers();
    vector<Crawler> getCrawlers() const;
    void displayBoard() const;
    void destroy();
    void loadCrawlersFromFile(const string& fname);
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