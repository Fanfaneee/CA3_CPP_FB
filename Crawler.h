#ifndef CRAWLER_H
#define CRAWLER_H

#include "Bug.h"

class Crawler : public Bug {
public:
    Crawler(int id, int x, int y, Direction dir, int size);
    Crawler(int id, int x, int y, Direction dir, int size);
    void move(int boardWidth, int boardHeight) override;
};

#endif // CRAWLER_H