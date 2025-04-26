#include "Bug.h"

Bug::Bug(int id, int x, int y, Direction dir, int size)
    : id(id), position({x, y}), direction(dir), size(size), alive(true) {
    path.push_back(position);
}

Bug::~Bug() = default;

int Bug::getId() const { return id; }
Position Bug::getPosition() const { return position; }
Direction Bug::getDirection() const { return direction; }
int Bug::getSize() const { return size; }
bool Bug::isAlive() const { return alive; }
const std::list<Position>& Bug::getPath() const { return path; }

void Bug::markAsDead() { alive = false; }

bool Bug::isWayBlocked(int boardWidth, int boardHeight) const {
    switch (direction) {
        case Direction::North: return (position.y == 0);
        case Direction::East:  return (position.x == boardWidth - 1);
        case Direction::South: return (position.y == boardHeight - 1);
        case Direction::West:  return (position.x == 0);
    }
    return false;
}
void Bug::grow(int sizeIncrease) {
    size += sizeIncrease;
}