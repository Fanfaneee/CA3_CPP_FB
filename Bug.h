#ifndef BUG_H
#define BUG_H

#include <list>
#include <iostream>

struct Position {
    int x;
    int y;

    bool operator<(const Position& other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
};

enum class Direction {
    North = 1,
    East = 2,
    South = 3,
    West = 4
};

class Bug {
protected:
    int id;
    Position position;
    Direction direction;
    int size;
    bool alive;
    std::list<Position> path;

public:
    Bug(int id, int x, int y, Direction dir, int size);
    virtual ~Bug();

    int getId() const;
    Position getPosition() const;
    Direction getDirection() const;
    int getSize() const;
    bool isAlive() const;
    const std::list<Position>& getPath() const;

    void markAsDead();
    bool isWayBlocked(int boardWidth, int boardHeight) const;

    virtual void move(int boardWidth, int boardHeight) = 0;
};

#endif // BUG_H