#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>
#include "SuperBug.h"

using namespace std;

Board::Board(int w, int h) : width(w), height(h) {}

Board::~Board() {
    for (auto bug : bugs) delete bug;
    for (auto bug : deadBugs) delete bug;
}

void Board::addBug(Bug* bug) {
    bugs.push_back(bug);
    updateCellOccupants();
}

void Board::loadCrawlersFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        stringstream ss(line);
        string temp;
        char type;
        int id, x, y, dir, size, hopLength;

        getline(ss, temp, ',');
        type = temp[0];
        getline(ss, temp, ',');
        id = stoi(temp);
        getline(ss, temp, ',');
        x = stoi(temp);
        getline(ss, temp, ',');
        y = stoi(temp);
        getline(ss, temp, ',');
        dir = stoi(temp);
        getline(ss, temp, ',');
        size = stoi(temp);

        if (type == 'C') {
            addBug(new Crawler(id, x, y, static_cast<Direction>(dir), size));
        } else if (type == 'H') {
            getline(ss, temp, ',');
            hopLength = stoi(temp);
            addBug(new Hopper(id, x, y, static_cast<Direction>(dir), size, hopLength));
        } else if (type == 'J') {
            getline(ss, temp, ',');
            int jumpRadius = stoi(temp);
            addBug(new Jumper(id, x, y, static_cast<Direction>(dir), size, jumpRadius));
        } else if (type == 'S') {
            addBug(new SuperBug(id, x, y, size));
        } else {
            cerr << "Unknown type in line: " << line << endl;
        }
    }

    file.close();
}
void Board::moveBugs() {
    for (auto& bug : bugs) {
        if (!dynamic_cast<SuperBug*>(bug)) {
            bug->move(width, height);
        }
    }
    updateCellOccupants();
}

void Board::displayAllBugs() const {
    if (bugs.empty()) {
        cout << "No bugs on the board.\n";
        return;
    }

    cout << left
         << setw(5) << "Id"
         << setw(10) << "Type"
         << setw(10) << "Position"
         << setw(10) << "Size"
         << setw(10) << "Direction"
         << setw(15) << "Special"
         << setw(10) << "Status" << endl;

    for (const auto& bug : bugs) {
        string type;
        string special;

        if (auto* superBug = dynamic_cast<SuperBug*>(bug)) {
            type = "SuperBug";
            special = "-";
        } else if (auto* jumper = dynamic_cast<Jumper*>(bug)) {
            type = "Jumper";
            special = "Radius: " + to_string(jumper->getJumpRadius());
        } else if (auto* hopper = dynamic_cast<Hopper*>(bug)) {
            type = "Hopper";
            special = "Hop: " + to_string(hopper->getHopLength());
        } else {
            type = "Crawler";
            special = "-";
        }

        cout << left
             << setw(5) << bug->getId()
             << setw(10) << type
             << setw(10) << "(" + to_string(bug->getPosition().x) + "," + to_string(bug->getPosition().y) + ")"
             << setw(10) << bug->getSize()
             << setw(10) << (bug->getDirection() == Direction::North ? "North" :
                             bug->getDirection() == Direction::East ? "East" :
                             bug->getDirection() == Direction::South ? "South" : "West")
             << setw(15) << special
             << setw(10) << (bug->isAlive() ? "Alive" : "Dead") << endl;
    }
}

void Board::findBug(int id) const {
    for (const auto& bug : bugs) {
        if (bug->getId() == id) {
            cout << "Bug found: ID=" << bug->getId() << ", Position=("
                 << bug->getPosition().x << "," << bug->getPosition().y << ")\n";
            return;
        }
    }
    cout << "Bug with ID=" << id << " not found.\n";
}

bool Board::tapBugBoard() {
    moveBugs();
    fight();
    return checkLastBugStanding();
}

void Board::fight() {
    random_device rd;
    mt19937 gen(rd());

    for (auto& [pos, occupants] : cellOccupants) {
        if (occupants.size() > 1) {
            Bug* strongest = occupants[0];
            vector<Bug*> contenders = {strongest};

            for (size_t i = 1; i < occupants.size(); ++i) {
                if (occupants[i]->getSize() > strongest->getSize()) {
                    strongest = occupants[i];
                    contenders = {strongest};
                } else if (occupants[i]->getSize() == strongest->getSize()) {
                    contenders.push_back(occupants[i]);
                }
            }

            if (contenders.size() > 1) {
                uniform_int_distribution<> dist(0, contenders.size() - 1);
                strongest = contenders[dist(gen)];
            }

            int totalSizeEaten = 0;
            vector<Bug*> toRemove;

            for (auto& bug : occupants) {
                if (bug != strongest) {
                    if (auto* superBug = dynamic_cast<SuperBug*>(bug)) {
                        if (strongest->getSize() >= superBug->getSize()) {
                            superBug->markAsDead();
                            toRemove.push_back(superBug);
                        } else {
                            totalSizeEaten += bug->getSize();
                        }
                    } else {
                        bug->markAsDead();
                        deadBugs.push_back(bug);
                        totalSizeEaten += bug->getSize();
                        toRemove.push_back(bug);
                    }
                }
            }

            strongest->grow(totalSizeEaten);

            for (auto& bug : toRemove) {
                occupants.erase(remove(occupants.begin(), occupants.end(), bug), occupants.end());
            }
        }
    }

    bugs.erase(remove_if(bugs.begin(), bugs.end(),
                         [](Bug* bug) { return !bug->isAlive(); }),
               bugs.end());
}

void Board::displayLifeHistory() const {
    for (const auto& bug : bugs) {
        cout << bug->getId() << " "
             << (dynamic_cast<SuperBug*>(bug) ? "SuperBug" :
                 dynamic_cast<Hopper*>(bug) ? "Hopper" :
                 dynamic_cast<Jumper*>(bug) ? "Jumper" : "Crawler")
             << " Path: ";
        for (const auto& pos : bug->getPath()) {
            cout << "(" << pos.x << "," << pos.y << ")";
            if (&pos != &bug->getPath().back()) {
                cout << ",";
            }
        }
        cout << " Alive" << endl;
    }

    for (const auto& bug : deadBugs) {
        cout << bug->getId() << " "
             << (dynamic_cast<SuperBug*>(bug) ? "SuperBug" :
                 dynamic_cast<Hopper*>(bug) ? "Hopper" :
                 dynamic_cast<Jumper*>(bug) ? "Jumper" : "Crawler")
             << " Path: ";
        for (const auto& pos : bug->getPath()) {
            cout << "(" << pos.x << "," << pos.y << ")";
            if (&pos != &bug->getPath().back()) {
                cout << ",";
            }
        }
        cout << " Eaten by " << bug->getId() << endl;
    }
}

void Board::updateCellOccupants() {
    cellOccupants.clear();
    for (auto& bug : bugs) {
        cellOccupants[bug->getPosition()].push_back(bug);
    }
}

void Board::displayAllCells() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position pos = {x, y};
            cout << "(" << x << "," << y << "): ";
            if (cellOccupants.find(pos) != cellOccupants.end() && !cellOccupants.at(pos).empty()) {
                const auto& occupants = cellOccupants.at(pos);
                for (size_t i = 0; i < occupants.size(); ++i) {
                    string type;
                    if (dynamic_cast<SuperBug*>(occupants[i])) {
                        type = "SuperBug";
                    } else if (dynamic_cast<Jumper*>(occupants[i])) {
                        type = "Jumper";
                    } else if (dynamic_cast<Hopper*>(occupants[i])) {
                        type = "Hopper";
                    } else {
                        type = "Crawler";
                    }

                    cout << type << " " << occupants[i]->getId();
                    if (i < occupants.size() - 1) {
                        cout << ", ";
                    }
                }
            } else {
                cout << "empty";
            }
            cout << endl;
        }
    }
}


SuperBug* Board::getSuperBugById(int id) const {
    for (auto* bug : bugs) {
        if (auto* superBug = dynamic_cast<SuperBug*>(bug)) {
            if (superBug->getId() == id) {
                return superBug;
            }
        }
    }
    return nullptr;
}



void Board::simulateGame() {
    while (!checkLastBugStanding()) {
        tapBugBoard();
        displayAllBugs();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "Simulation complete. Last bug standing!\n";
}

void Board::writeLifeHistoryToFile() const {
    ofstream file("life_history.txt");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file for writing.\n";
        return;
    }

    for (const auto& bug : bugs) {
        file << "Bug ID=" << bug->getId() << " Path: ";
        for (const auto& pos : bug->getPath()) {
            file << "(" << pos.x << "," << pos.y << ")";
            if (&pos != &bug->getPath().back()) {
                file << ",";
            }
        }
        file << " Alive\n";
    }

    for (const auto& bug : deadBugs) {
        file << "Bug ID=" << bug->getId() << " Path: ";
        for (const auto& pos : bug->getPath()) {
            file << "(" << pos.x << "," << pos.y << ")";
            if (&pos != &bug->getPath().back()) {
                file << ",";
            }
        }
        file << " Dead\n";
    }

    file.close();
    cout << "Life history written to file.\n";
}

vector<Bug*> Board::getBugs() const {
    return bugs;
}

bool Board::checkLastBugStanding() const {
    int aliveCount = 0;
    for (const auto& bug : bugs) {
        if (bug->isAlive()) {
            aliveCount++;
            if (aliveCount > 1) {
                return false;
            }
        }
    }
    return aliveCount == 1;
}