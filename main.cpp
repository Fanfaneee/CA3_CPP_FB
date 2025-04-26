#include <iostream>
#include <vector>
#include "Crawler.h"
#include "Board.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

void displayMenu() {
    cout << "Menu:\n";
    cout << "1. Initialize Bug Board (load data from file)\n";
    cout << "2. Display all Bugs\n";
    cout << "3. Find a Bug (given an id)\n";
    cout << "4. Tap the Bug Board (cause all to move, then fight/eat)\n";
    cout << "5. Display Life History of all Bugs (path taken)\n";
    cout << "6. Display all Cells listing their Bugs\n";
    cout << "7. Run simulation (generates a Tap every tenth of a second without superbug)\n";
    cout << "8. Run simulation sfml(with superbug)\n";
    cout << "9. Exit (write Life History of all Bugs to file)\n";

    cout << "Enter your choice: ";
}

int main() {
    Board board(10, 10);
    SuperBug* superBug = nullptr;
    int choice;
    bool exit = false;

    while (!exit) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                board.loadCrawlersFromFile("../crawler-bug.txt");
                superBug = board.getSuperBugById(200);
                if (superBug) {
                    cout << "SuperBug loaded successfully.\n";
                } else {
                    cout << "SuperBug not found in the file.\n";
                }
                break;
            case 2:
                board.displayAllBugs();
                break;
            case 3: {
                int id;
                cout << "Enter Bug ID: ";
                cin >> id;
                board.findBug(id);
                break;
            }
            case 4:
                board.tapBugBoard();
                board.displayAllBugs();
                break;
            case 5:
                board.displayLifeHistory();
                break;
            case 6:
                board.displayAllCells();
                break;
            case 7:
                board.simulateGame();
                break;
            case 8:
                if (superBug) {
                    RenderWindow window(VideoMode(600, 600), "Bug Simulation with SuperBug");
                    window.setFramerateLimit(60);

                    Font font;
                    if (!font.loadFromFile("ARIAL.TTF")) {
                        cerr << "Erreur : Impossible de charger la police ARIAL.TTF" << endl;
                        return 1;
                    }

                    const int cellSize = 60;
                    const int boardWidth = 10;
                    const int boardHeight = 10;

                    vector<RectangleShape> grid;
                    for (int x = 0; x < boardWidth; x++) {
                        for (int y = 0; y < boardHeight; y++) {
                            RectangleShape cell(Vector2f(cellSize, cellSize));
                            cell.setPosition(x * cellSize, y * cellSize);
                            cell.setFillColor((x + y) % 2 == 0 ? Color::Black : Color::White);
                            grid.push_back(cell);
                        }
                    }

                    bool gameOver = false;
                    Bug* lastBug = nullptr;

                    while (window.isOpen()) {
                        Event event;
                        while (window.pollEvent(event)) {
                            if (event.type == Event::Closed) {
                                window.close();
                            }

                            if (!gameOver && event.type == Event::KeyPressed) {
                                switch (event.key.code) {
                                    case Keyboard::Up:
                                        superBug->setDirection(Direction::North);
                                        superBug->move(boardWidth, boardHeight);
                                        break;
                                    case Keyboard::Down:
                                        superBug->setDirection(Direction::South);
                                        superBug->move(boardWidth, boardHeight);
                                        break;
                                    case Keyboard::Left:
                                        superBug->setDirection(Direction::West);
                                        superBug->move(boardWidth, boardHeight);
                                        break;
                                    case Keyboard::Right:
                                        superBug->setDirection(Direction::East);
                                        superBug->move(boardWidth, boardHeight);
                                        break;
                                }
                                board.tapBugBoard();
                            }
                        }

                        if (!gameOver && board.checkLastBugStanding()) {
                            gameOver = true;
                            for (const auto& bug : board.getBugs()) {
                                if (bug->isAlive()) {
                                    lastBug = bug;
                                    break;
                                }
                            }
                        }

                        window.clear();


                        for (const auto& cell : grid) {
                            window.draw(cell);
                        }

                        for (const auto& bug : board.getBugs()) {
                            CircleShape bugShape(cellSize / 3);
                            bugShape.setPosition(bug->getPosition().x * cellSize + cellSize / 6,
                                                 bug->getPosition().y * cellSize + cellSize / 6);


                            Text bugText, sizeText;
                            bugText.setFont(font);
                            sizeText.setFont(font);
                            bugText.setCharacterSize(14);
                            sizeText.setCharacterSize(12);
                            bugText.setFillColor(Color::Black);
                            sizeText.setFillColor(Color::Black);

                            if (dynamic_cast<Crawler*>(bug)) {
                                bugShape.setFillColor(Color::Yellow);
                                bugText.setString("C");
                            } else if (dynamic_cast<Hopper*>(bug)) {
                                bugShape.setFillColor(Color::Blue);
                                bugText.setString("H");
                            } else if (dynamic_cast<Jumper*>(bug)) {
                                bugShape.setFillColor(Color::Red);
                                bugText.setString("J");
                            } else if (dynamic_cast<SuperBug*>(bug)) {
                                bugShape.setFillColor(Color::Green);
                                bugText.setString("S");
                            }

                            sizeText.setString(to_string(bug->getSize()));


                            FloatRect bugTextBounds = bugText.getLocalBounds();
                            bugText.setOrigin(bugTextBounds.width / 2, bugTextBounds.height / 2);
                            bugText.setPosition(bugShape.getPosition().x + bugShape.getRadius(),
                                                bugShape.getPosition().y + bugShape.getRadius() - 5);

                            FloatRect sizeTextBounds = sizeText.getLocalBounds();
                            sizeText.setOrigin(sizeTextBounds.width / 2, sizeTextBounds.height / 2);
                            sizeText.setPosition(bugShape.getPosition().x + bugShape.getRadius(),
                                                 bugShape.getPosition().y + bugShape.getRadius() + 10);

                            window.draw(bugShape);
                            window.draw(bugText);
                            window.draw(sizeText);
                        }


                        CircleShape superBugShape(cellSize / 3);
                        superBugShape.setPosition(superBug->getPosition().x * cellSize + cellSize / 6,
                                                  superBug->getPosition().y * cellSize + cellSize / 6);
                        superBugShape.setFillColor(Color::Green);

                        Text superBugText, superBugSizeText;
                        superBugText.setFont(font);
                        superBugSizeText.setFont(font);
                        superBugText.setCharacterSize(14);
                        superBugSizeText.setCharacterSize(12);
                        superBugText.setFillColor(Color::Black);
                        superBugSizeText.setFillColor(Color::Black);

                        superBugText.setString("S");
                        superBugSizeText.setString(to_string(superBug->getSize()));

                        FloatRect superBugTextBounds = superBugText.getLocalBounds();
                        superBugText.setOrigin(superBugTextBounds.width / 2, superBugTextBounds.height / 2);
                        superBugText.setPosition(superBugShape.getPosition().x + superBugShape.getRadius(),
                                                 superBugShape.getPosition().y + superBugShape.getRadius() - 5);

                        FloatRect superBugSizeTextBounds = superBugSizeText.getLocalBounds();
                        superBugSizeText.setOrigin(superBugSizeTextBounds.width / 2, superBugSizeTextBounds.height / 2);
                        superBugSizeText.setPosition(superBugShape.getPosition().x + superBugShape.getRadius(),
                                                      superBugShape.getPosition().y + superBugShape.getRadius() + 10);

                        window.draw(superBugShape);
                        window.draw(superBugText);
                        window.draw(superBugSizeText);
                        if (gameOver && lastBug) {
                            RectangleShape messageBox(Vector2f(400, 200));
                            messageBox.setFillColor(Color::Black);
                            messageBox.setOutlineColor(Color::Blue);
                            messageBox.setOutlineThickness(5);
                            messageBox.setPosition(100, 200);

                            Text message;
                            message.setFont(font);
                            message.setCharacterSize(20);
                            message.setFillColor(Color::White);
                            message.setString(std::string("Le dernier bug est :\n") +
                   (dynamic_cast<SuperBug*>(lastBug) ? "SuperBug" :
                    dynamic_cast<Hopper*>(lastBug) ? "Hopper" :
                    dynamic_cast<Jumper*>(lastBug) ? "Jumper" : "Crawler") +
                   "\nID: " + std::to_string(lastBug->getId()) +
                   "\nTaille: " + std::to_string(lastBug->getSize()));
                            message.setPosition(120, 240);

                            window.draw(messageBox);
                            window.draw(message);
                        }

                        window.display();
                    }
                } else {
                    cout << "SuperBug not available for GUI.\n";
                }
                break;
            case 9:
                board.writeLifeHistoryToFile();
                cout << "Life History written to file. Closing...\n";
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}