#include "Board.h"
#include <iostream>
using namespace std;

Board::Board(int w, int h) : width(w), height(h) {}

void Board::addCrawler(int id, int x, int y, Direction dir, int size) {
    crawlers.push_back(new Crawler(id, x, y, dir, size));
}

void Board::moveCrawlers() {
    for (auto& bug : crawlers) {
        bug->move(width, height);
    }
}

vector<Crawler> Board::getCrawlers() const {
    vector<Crawler> copy;
    for (const auto& bug : crawlers) {
        copy.push_back(*bug);
    }
    return copy;
}

void Board::displayBoard() const {
    cout << "Bugs on the board:\n";
    for (const auto& bug : crawlers) {
        cout << "Bug ID: " << bug->id
                  << " at (" << bug->position.x << ", " << bug->position.y << ")\n";
    }
}

void Board::destroy() {
    for (auto bug : crawlers) {
        delete bug;
    }
    crawlers.clear();
}
