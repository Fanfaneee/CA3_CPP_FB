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
    cout << "7. Run simulation (generates a Tap every tenth of a second)\n";
    cout << "8. Exit (write Life History of all Bugs to file)\n";
    cout << "Enter your choice: ";
}

int main() {
    Board board(10, 10);
    int choice;
    bool exit = false;

    while (!exit) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                board.loadCrawlersFromFile("../crawler-bug.txt");
            cout << "Bug Board initialized.\n";
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
                board.writeLifeHistoryToFile();
                cout << "Life History written to file. Launching GUI...\n";

                // Lancer l'interface graphique
            {
                RenderWindow window(VideoMode(600, 600), "Bug Simulation");
                window.setFramerateLimit(60);

                // Charger la police
                Font font;
                if (!font.loadFromFile("ARIAL.TTF")) {
                    cerr << "Erreur : Impossible de charger la police ARIAL.TTF" << endl;
                    break;
                }

                // Plateau de jeu
                const int cellSize = 60;
                const int boardWidth = 10;
                const int boardHeight = 10;

                // Créer la grille
                vector<RectangleShape> grid;
                for (int x = 0; x < boardWidth; x++) {
                    for (int y = 0; y < boardHeight; y++) {
                        RectangleShape cell(Vector2f(cellSize, cellSize));
                        cell.setPosition(x * cellSize, y * cellSize);
                        cell.setFillColor((x + y) % 2 == 0 ? Color::Black : Color::White);
                        grid.push_back(cell);
                    }
                }

                while (window.isOpen()) {
                    Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed) {
                            window.close();
                        }
                    }

                    // Mettre à jour les insectes
                    board.tapBugBoard();

                    // Dessiner la fenêtre
                    window.clear();

                    // Dessiner la grille
                    for (const auto& cell : grid) {
                        window.draw(cell);
                    }

                    // Dessiner les insectes
                    for (const auto& bug : board.getBugs()) {
                        CircleShape bugShape(cellSize / 3);
                        bugShape.setPosition(bug->getPosition().x * cellSize + cellSize / 6,
                                             bug->getPosition().y * cellSize + cellSize / 6);

                        // Définir la couleur et le texte en fonction du type d'insecte
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
                        }

                        sizeText.setString(to_string(bug->getSize()));

                        // Positionner le texte
                        bugText.setPosition(bugShape.getPosition().x + cellSize / 6,
                                            bugShape.getPosition().y + cellSize / 6);
                        sizeText.setPosition(bugShape.getPosition().x + cellSize / 6,
                                             bugShape.getPosition().y + cellSize / 2);

                        // Dessiner l'insecte et son texte
                        window.draw(bugShape);
                        window.draw(bugText);
                        window.draw(sizeText);
                    }

                    window.display();
                }
            }
            case 9:
                board.writeLifeHistoryToFile();
            cout << "Life History written to file. Closingg...\n";
            exit = true;
            break;
            default:
                cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    return 0;
}