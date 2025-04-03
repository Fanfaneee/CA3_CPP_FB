#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Crawler.h"
using namespace std;

class Board {
private:
    int width;
    int height;
    vector<Crawler*> crawlers;

public:
    Board(int w, int h);

    void addCrawler(int id, int x, int y, Direction dir, int size);
    void moveCrawlers();
    vector<Crawler> getCrawlers() const;
    void displayBoard() const;
    void destroy();
    void loadCrawlersFromFile(const string& fname);
    void displayAllBugs() const;

};

#endif // BOARD_H