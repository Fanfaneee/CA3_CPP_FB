#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

struct CrawlerData {
    int id;
    int x;
    int y;
    Direction direction;
    int size;
};


void parseLine(const string& line, CrawlerData &data) {
    string temp;
    stringstream ss(line);

    getline(ss, temp, ','); // Ignore le 'C'
    getline(ss, temp, ',');
    data.id = stoi(temp);
    getline(ss, temp, ',');
    data.x = stoi(temp);
    getline(ss, temp, ',');
    data.y = stoi(temp);
    getline(ss, temp, ',');
    data.direction = static_cast<Direction>(stoi(temp));
    getline(ss, temp, ',');
    data.size = stoi(temp);
}

void Board::loadCrawlersFromFile(const string& fname) {
    ifstream fin(fname);

    if (fin) {
        string line;
        while (getline(fin, line)) {
            CrawlerData data{};
            parseLine(line, data);
            crawlers.push_back(new Crawler(data.id, data.x, data.y, data.direction, data.size));
        }
        fin.close();
    } else {
        cout << "Error with oppening of the file." << endl;
    }
}
