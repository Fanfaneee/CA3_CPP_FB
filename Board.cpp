#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>

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
            cout << "Crawler loaded: ID=" << id << ", Position=(" << x << "," << y << "), Direction=" << dir << ", Size=" << size << endl;
        } else if (type == 'H') {
            getline(ss, temp, ',');
            hopLength = stoi(temp);
            addBug(new Hopper(id, x, y, static_cast<Direction>(dir), size, hopLength));
            cout << "Hopper loaded: ID=" << id << ", Position=(" << x << "," << y << "), Direction=" << dir << ", Size=" << size << ", Hop Length=" << hopLength << endl;
        }else if (type == 'J') {
            getline(ss, temp, ',');
            int jumpRadius = stoi(temp);
            addBug(new Jumper(id, x, y, static_cast<Direction>(dir), size, jumpRadius));
            cout << "Jumper loaded: ID=" << id << ", Position=(" << x << "," << y << "), Direction=" << dir << ", Size=" << size << ", Jump Radius=" << jumpRadius << endl;
        }
        else {
            cerr << "Unknown type in line: " << line << endl;
        }
    }

    file.close();
}

void Board::moveBugs() {
    for (auto& bug : bugs) {
        bug->move(width, height);
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

        if (auto* jumper = dynamic_cast<Jumper*>(bug)) {
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
            cout << "Bug ID: " << bug->getId()
                 << " Position: (" << bug->getPosition().x << "," << bug->getPosition().y << ")"
                 << " Size: " << bug->getSize()
                 << " Status: " << (bug->isAlive() ? "Alive" : "Dead") << endl;
            return;
        }
    }
    cout << "Bug " << id << " not found" << endl;
}

bool Board::tapBugBoard() {
    moveBugs();
    fight();
    updateCellOccupants();
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
            for (auto& bug : occupants) {
                if (bug != strongest) {
                    bug->markAsDead();
                    deadBugs.push_back(bug);
                    totalSizeEaten += bug->getSize();
                }
            }

            strongest->grow(totalSizeEaten);
        }
    }

    bugs.erase(remove_if(bugs.begin(), bugs.end(),
                         [](Bug* bug) { return !bug->isAlive(); }),
               bugs.end());
}


void Board::displayLifeHistory() const {
    for (const auto& bug : bugs) {
        cout << bug->getId() << " "
             << (dynamic_cast<Hopper*>(bug) ? "Hopper" : "Crawler")
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
             << (dynamic_cast<Hopper*>(bug) ? "Hopper" : "Crawler")
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
void Board::writeLifeHistoryToFile() const {
    ofstream file("bugs_life_history.txt");
    if (!file.is_open()) {
        cerr << "Error: Unable to write to file." << endl;
        return;
    }

    for (const auto& bug : bugs) {
        file << "Bug ID: " << bug->getId() << " Path: ";
        for (const auto& pos : bug->getPath()) {
            file << "(" << pos.x << "," << pos.y << ") ";
        }
        file << endl;
    }

    file.close();
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
                    if (dynamic_cast<Jumper*>(occupants[i])) {
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

bool Board::checkLastBugStanding() const {
    return bugs.size() == 1;
}

void Board::simulateGame() {
    while (!tapBugBoard()) {
        displayAllBugs();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    displayAllBugs();
    writeLifeHistoryToFile();
    cout << "Simulation complete. Results written to file." << endl;
}