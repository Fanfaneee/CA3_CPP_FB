#include <iostream>
using namespace std;

//#include "Board.h"
#include "Crawler.h"


Crawler::Crawler(int id, int x, int y, Direction dir, int size) {
    this->id = id;
    this->position = {x, y};
    this->direction = dir;
    this->size = size;
    this->alive = true;

    path.push_back(position);
}

void Crawler::move(int boardWidth, int boardHeight) {
    if (!alive) return;


    if (isWayBlocked(boardWidth, boardHeight)) {
        do {
            direction = static_cast<Direction>((rand() % 4) + 1);
        } while (isWayBlocked(boardWidth, boardHeight));
    }

    switch (direction) {
        case Direction::North:
            if (position.y > 0) position.y--;
        break;
        case Direction::East:
            if (position.x < boardWidth - 1) position.x++;
        break;
        case Direction::South:
            if (position.y < boardHeight - 1) position.y++;
        break;
        case Direction::West:
            if (position.x > 0) position.x--;
        break;
    }

    path.push_back(position);
}

bool Crawler::isWayBlocked(int boardWidth, int boardHeight) const {
    switch (direction) {
        case Direction::North: return (position.y == 0);
        case Direction::East:  return (position.x == boardWidth - 1);
        case Direction::South: return (position.y == boardHeight - 1);
        case Direction::West:  return (position.x == 0);
    }
    return false;
}