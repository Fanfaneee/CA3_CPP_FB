#include "SuperBug.h"

SuperBug::SuperBug(int id, int x, int y, int size)
    : Bug(id, x, y, Direction::North, size) {}

void SuperBug::move(int boardWidth, int boardHeight) {
    if (!alive) return;

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