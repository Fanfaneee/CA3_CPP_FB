#include "Hopper.h"
#include <cstdlib>

Hopper::Hopper(int id, int x, int y, Direction dir, int size, int hopLength)
    : Bug(id, x, y, dir, size), hopLength(hopLength) {}

void Hopper::move(int boardWidth, int boardHeight) {
    if (!alive) return;

    if (isWayBlocked(boardWidth, boardHeight)) {
        do {
            direction = static_cast<Direction>((rand() % 4) + 1);
        } while (isWayBlocked(boardWidth, boardHeight));
    }

    int newX = position.x;
    int newY = position.y;

    switch (direction) {
        case Direction::North:
            newY = position.y - hopLength;
        if (newY < 0) newY = 0;
        break;
        case Direction::East:
            newX = position.x + hopLength;
        if (newX >= boardWidth) newX = boardWidth - 1;
        break;
        case Direction::South:
            newY = position.y + hopLength;
        if (newY >= boardHeight) newY = boardHeight - 1;
        break;
        case Direction::West:
            newX = position.x - hopLength;
        if (newX < 0) newX = 0;
        break;
    }

    position = {newX, newY};
    path.push_back(position);
}