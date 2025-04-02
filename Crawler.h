#ifndef CRAWLER_H
#define CRAWLER_H

#include <iostream>
#include <list>

struct Position {
    int x;
    int y;
};

enum class Direction {
    North = 1,
    East = 2,
    South = 3,
    West = 4
};

class Crawler {
public:
    int id;
    Position position;
    Direction direction;
    int size;
    bool alive;
    std::list<Position> path;


    Crawler(int id, int x, int y, Direction dir, int size);


    void move(int boardWidth, int boardHeight);


    bool isWayBlocked(int boardWidth, int boardHeight) const;
};

#endif // CRAWLER_H
