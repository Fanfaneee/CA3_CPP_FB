#include <iostream>
#include <vector>
#include "Crawler.h"
#include "Board.h"
using namespace std;

int main() {


    Board board(10, 10);

    board.loadCrawlersFromFile("../crawler-bug.txt");
    board.displayAllBugs();


    return 0;

}
