/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "simpio.h"
#include "maze-types.h"
#include "maze-graphics.h"
#include "set.h"
#include "random.h"

const int TIME_DELAY = 2000;

void initialize(MazeGeneratorView& maze, const int dimension);

Vector<cell> createCells(int dimension);

Vector<wall> buildWalls(const Vector<cell> cells, int dimension);

Vector<Set<cell>> buildChambers(const Vector<cell> cells);

void printCells(Vector<cell>& cells);

void printWalls(Vector<wall>& walls);

void removeRandomWall(Vector<wall>& walls, int marked, Vector<Set<cell>>& chambers, MazeGeneratorView& maze);

Vector<wall> shuffleWalls(Vector<wall>& walls);



static int getMazeDimension(string prompt,
                            int minDimension = 7, int maxDimension = 50) {
    while (true) {
        int response = getInteger(prompt);
        if (response == 0) return response;
        if (response >= minDimension && response <= maxDimension) return response;
        cout << "Please enter a number between "
             << minDimension << " and "
             << maxDimension << ", inclusive." << endl;
    }
}

int main() {
    while (true) {
        int dimension = getMazeDimension("What should the dimension of your maze be [0 to exit]? ");
        if (dimension == 0) break;
        cout << "This is where I'd animate the construction of a maze of dimension " << dimension << "." << endl;

        // Initialize a new maze with size dimension
        MazeGeneratorView maze;
        initialize(maze, dimension);
        // Generate list of cells
        Vector<cell> cells = createCells(dimension);
        // Generate list of walls
        cout << "Generating walls..." << endl;
        Vector<wall> walls = buildWalls(cells, dimension);
        pause(TIME_DELAY);
        Vector<wall> shuffledWalls = shuffleWalls(walls);
        cout << "Building chambers..." << endl;
        Vector<Set<cell>> chambers = buildChambers(cells);
        maze.addAllWalls(shuffledWalls);
        maze.repaint();
        pause(TIME_DELAY);
        cout << "Mapping Maze..." << endl;
        for (int j = 0; j < shuffledWalls.size(); ++j) {
            removeRandomWall(shuffledWalls, j, chambers, maze);
            maze.repaint();
            // pause(TIME_DELAY);
        }

        //
        pause(TIME_DELAY*30);
    }
    return 0;
}

void initialize(MazeGeneratorView& maze, const int dimension) {
    MazeGeneratorView();
    maze.setDimension(dimension);
    maze.drawBorder();
}

Vector<cell> createCells(int dimension) {
    Vector<cell> chamber;
    for (int r = 0; r < dimension; ++r) {
        for (int c = 0; c < dimension; ++c) {
            cell cel;
            cel.col = c;
            cel.row = r;
            chamber.add(cel);
        }
    }
    return chamber;
}

Vector<wall> buildWalls(const Vector<cell> cells, int dimension) {
    Vector<wall> walls;
    for (int i = 0; i < cells.size(); ++i) {
        if (cells[i].col < dimension-1) {
            wall w;
            w.one = cells[i];
            w.two = cells[i+1];
            walls.add(w);
        }
        if (cells[i].row < dimension-1) {
            wall b;
            b.one = cells[i];
            b.two = cells[i+dimension];
            walls.add(b);
        }
    }
    return walls;
}

void printCells(Vector<cell>& cells) {
    for (int i = 0; i < cells.size(); ++i) {
        cout << "(";
        cout << cells[i].row << "," << cells[i].col;
        cout << ")"<< endl;
    }
}

void printWalls(Vector<wall>& walls) {
    for (int i = 0; i < walls.size(); ++i) {
        cout << "(";
        cout << walls[i].one.col << "," << walls[i].one.row;
        cout << ")"<< ",(";
        cout << walls[i].two.col << "," << walls[i].two.row;
        cout << ")" << endl;
    }
}

void removeRandomWall(Vector<wall>& walls, int marked, Vector<Set<cell>>& chambers, MazeGeneratorView& maze) {
    wall chamberWall = walls.get(marked);
    cell chamberOne = chamberWall.one;
    cell chamberTwo = chamberWall.two;
    Set<cell> chamberSetOne;
    Set<cell> chamberSetTwo;
    for (int i = 0; i < chambers.size(); ++i) {
        if (chambers[i].contains(chamberOne)) {
            chamberSetOne = chambers[i];
        }
        if (chambers[i].contains(chamberTwo)) {
            chamberSetTwo = chambers[i];
        }
    }
    if (chamberSetOne != chamberSetTwo) {
        Set<cell> newSet = chamberSetOne + chamberSetTwo;
        chambers.add(newSet);
        maze.removeWall(chamberWall);
    }
}

// Return Set of Sets of cells which represent chambers
Vector<Set<cell>> buildChambers(const Vector<cell> cells) {
    Vector<Set<cell>> chambers;
    for (int i = 0; i < cells.size(); ++i) {
        Set<cell> chamber;
        chamber.add(cells[i]);
        chambers.add(chamber);
    }
    return chambers;
}

Vector<wall> shuffleWalls(Vector<wall>& walls) {
    Vector<wall> newWalls;
    while (!walls.isEmpty()) {
        int rand = randomInteger(0,walls.size()-1);
        newWalls.add(walls.get(rand));
        walls.remove(rand);
    }
    return newWalls;
}

