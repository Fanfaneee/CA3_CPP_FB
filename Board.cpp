#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <random>


using namespace std;

Board::Board(int w, int h) : width(w), height(h) {}

void Board::addCrawler(int id, int x, int y, Direction dir, int size) {
    crawlers.push_back(new Crawler(id, x, y, dir, size));
    majCellOccupants();
}

void Board::moveCrawlers() {
    for (auto& bug : crawlers) {
        bug->move(width, height);
    }
    majCellOccupants();
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
    for (auto bug : deadCrawlers) {
        delete bug;
    }
    crawlers.clear();
    deadCrawlers.clear();
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

    getline(ss, temp, ',');
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
    cout << "Bug " << id << " nott found" << endl;
}

void Board::tapBugBoard() {
    for (auto& bug : crawlers) {
        bug->move(width, height);
    }
    fight();
    majCellOccupants();
}
void Board::fight() {
    random_device rd;
    mt19937 gen(rd());

    for (size_t i = 0; i < crawlers.size(); ++i) {
        for (size_t j = i + 1; j < crawlers.size(); ++j) {
            if (crawlers[i]->position.x == crawlers[j]->position.x &&
                crawlers[i]->position.y == crawlers[j]->position.y) {
                if (crawlers[i]->size > crawlers[j]->size) {
                    crawlers[i]->size += crawlers[j]->size;
                    crawlers[j]->alive = false;
                    crawlers[j]->eatenBy = crawlers[i]->id;
                    deadCrawlers.push_back(crawlers[j]);
                } else if (crawlers[i]->size < crawlers[j]->size) {
                    crawlers[j]->size += crawlers[i]->size;
                    crawlers[i]->alive = false;
                    crawlers[i]->eatenBy = crawlers[j]->id;
                    deadCrawlers.push_back(crawlers[i]);
                } else {
                    uniform_int_distribution<> dis(0, 1);
                    if (dis(gen) == 0) {
                        crawlers[i]->size += crawlers[j]->size;
                        crawlers[j]->alive = false;
                        crawlers[j]->eatenBy = crawlers[i]->id;
                        deadCrawlers.push_back(crawlers[j]);
                    } else {
                        crawlers[j]->size += crawlers[i]->size;
                        crawlers[i]->alive = false;
                        crawlers[i]->eatenBy = crawlers[j]->id;
                        deadCrawlers.push_back(crawlers[i]);
                    }
                }
                }
        }
    }
    crawlers.erase(remove_if(crawlers.begin(), crawlers.end(),
                             [](const Crawler* bug) { return !bug->alive; }),
                   crawlers.end());
}

void Board::displayLifeHistory() const {
    for (const auto& bug : crawlers) {
        cout << "Bug ID: " << bug->id << " Crawler Path: ";
        for (const auto& pos : bug->path) {
            cout << "(" << pos.x << "," << pos.y << ")";
            if (&pos != &bug->path.back()) {
                cout << ",";
            }
        }
        cout << endl;
    }
    for (const auto& bug : deadCrawlers) {
        cout << "Bug ID: " << bug->id << " Crawler Path: ";
        for (const auto& pos : bug->path) {
            cout << "(" << pos.x << "," << pos.y << ")";
            if (&pos != &bug->path.back()) {
                cout << ",";
            }
        }
        cout << " Eaten by " << bug->eatenBy << endl;
    }
}

void Board::writeLifeHistoryToFile() const {
    string filename = "bugs_life_history.out";
    ofstream fout(filename);

    if (fout) {
        for (const auto& bug : crawlers) {
            fout << "Bug ID: " << bug->id << " Crawler Path: ";
            for (const auto& pos : bug->path) {
                fout << "(" << pos.x << "," << pos.y << ")";
                if (&pos != &bug->path.back()) {
                    fout << ",";
                }
            }
            fout << endl;
        }
        for (const auto& bug : deadCrawlers) {
            fout << "Bug ID: " << bug->id << " Crawler Path: ";
            for (const auto& pos : bug->path) {
                fout << "(" << pos.x << "," << pos.y << ")";
                if (&pos != &bug->path.back()) {
                    fout << ",";
                }
            }
            fout << " Eaten by " << bug->eatenBy << endl;
        }
        fout.close();
    } else {
        cout << "Error openning file for writing." << endl;
    }
}

void Board::majCellOccupants() {
    cellOccupants.clear();
    for (auto& bug : crawlers) {
        cellOccupants[bug->position].push_back(bug);
    }
}

void Board::displayAllCells() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position pos = {x, y};
            cout << "(" << x << "," << y << "): ";
            if (cellOccupants.find(pos) != cellOccupants.end()) {
                for (const auto& bug : cellOccupants.at(pos)) {
                    cout << "Crawler " << bug->id << ", ";
                }
                cout << "\b\b  ";
            } else {
                cout << "empty";
            }
            cout << endl;
        }
    }
}