#include "Crawler.h"
#include <cstdlib>

Crawler::Crawler(int id, int x, int y, Direction dir, int size)
    : Bug(id, x, y, dir, size) {}

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