//
// Created by fanfa on 26/04/2025.
//

#ifndef JUMPER_H
#define JUMPER_H

#include "Bug.h"

class Jumper : public Bug {
private:
    int jumpRadius;

public:
    Jumper(int id, int x, int y, Direction dir, int size, int jumpRadius);
    void move(int boardWidth, int boardHeight) override;
    int getJumpRadius() const;
};

#endif // JUMPER_H