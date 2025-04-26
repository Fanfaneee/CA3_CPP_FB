#ifndef SUPERBUG_H
#define SUPERBUG_H

#include "Bug.h"

class SuperBug : public Bug {
public:
    SuperBug(int id, int x, int y, int size);
    void move(int boardWidth, int boardHeight) override;
};

#endif // SUPERBUG_H