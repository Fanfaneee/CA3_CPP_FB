#include <iostream>
#include <vector>
#include "Crawler.h"

int main() {

    int boardWidth = 10;
    int boardHeight = 10;


    Crawler* bug = new Crawler(101, 5, 5, Direction::East, 3);

    std::cout << "Position initiale du bug : (" << bug->position.x << ", " << bug->position.y << ")\n";


    for (int i = 0; i < 5; i++) {
        bug->move(boardWidth, boardHeight);
        std::cout << "Après déplacement " << i + 1 << " : (" << bug->position.x << ", " << bug->position.y << ")\n";
    }

    delete bug;

    return 0;
}
