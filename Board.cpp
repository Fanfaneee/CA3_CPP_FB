#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

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

void Board::displayAllBugs() const {
    cout << left
         << setw(5)  << "Id"
         << setw(10) << "Crawler"
         << setw(10) << "Position"
         << setw(10) << "Direction"
         << setw(10) << "Size"
         << setw(10) << "Status" << endl;
    for (const auto& bug : crawlers) {
        cout << left
             << setw(5)  << bug->id
             << setw(10) << "Crawler"
             << setw(10) << (to_string(bug->position.x) + "," + to_string(bug->position.y));
        switch (bug->direction) {
            case Direction::North: cout << setw(10) << "North"; break;
            case Direction::East:  cout << setw(10) << "East"; break;
            case Direction::South: cout << setw(10) << "South"; break;
            case Direction::West:  cout << setw(10) << "West"; break;
        }
        cout << setw(10) << bug->size
             << setw(10) << (bug->alive ? "Alive" : "Dead") << endl;
    }
}

void Board::findBug(int id) const {
    for (const auto& bug : crawlers) {
        if (bug->id == id) {
            cout << "Bug ID: " << bug->id
                 << " Position: (" << bug->position.x << "," << bug->position.y << ")"
                 << " Direction: ";
            switch (bug->direction) {
                case Direction::North: cout << "North"; break;
                case Direction::East:  cout << "East"; break;
                case Direction::South: cout << "South"; break;
                case Direction::West:  cout << "West"; break;
            }
            cout << " Size: " << bug->size
                 << " Status: " << (bug->alive ? "Alive" : "Dead") << endl;
            return;
        }
    }
    cout << "Bug " << id << " not found." << endl;
}
void Board::tapBugBoard() {
    for (auto& bug : crawlers) {
        bug->move(width, height);
    }
    fight();
}
void Board::fight() {
    for (size_t i = 0; i < crawlers.size(); ++i) {
        for (size_t j = i + 1; j < crawlers.size(); ++j) {
            if (crawlers[i]->position.x == crawlers[j]->position.x &&
                crawlers[i]->position.y == crawlers[j]->position.y) {
                if (crawlers[i]->size > crawlers[j]->size) {
                    crawlers[i]->size += crawlers[j]->size;
                    crawlers[j]->alive = false;
                } else if (crawlers[i]->size < crawlers[j]->size) {
                    crawlers[j]->size += crawlers[i]->size;
                    crawlers[i]->alive = false;
                }
                }
        }
    }
    // Supprimer les bugs morts
    crawlers.erase(remove_if(crawlers.begin(), crawlers.end(),
                             [](const Crawler* bug) { return !bug->alive; }),
                   crawlers.end());
}