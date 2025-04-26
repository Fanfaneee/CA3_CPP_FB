//
// Created by fanfa on 26/04/2025.
//

#include "Jumper.h"
#include <cstdlib>

Jumper::Jumper(int id, int x, int y, Direction dir, int size, int jumpRadius)
    : Bug(id, x, y, dir, size), jumpRadius(jumpRadius) {}

void Jumper::move(int boardWidth, int boardHeight) {
    if (!alive) return;

    int newX = position.x + (rand() % (2 * jumpRadius + 1)) - jumpRadius;
    int newY = position.y + (rand() % (2 * jumpRadius + 1)) - jumpRadius;

    if (newX < 0) newX = 0;
    if (newX >= boardWidth) newX = boardWidth - 1;
    if (newY < 0) newY = 0;
    if (newY >= boardHeight) newY = boardHeight - 1;

    position = {newX, newY};
    path.push_back(position);
}

int Jumper::getJumpRadius() const {
    return jumpRadius;
}