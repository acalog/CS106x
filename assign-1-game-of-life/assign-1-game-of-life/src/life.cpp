/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <fstream>   // for file read

using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "gtimer.h"  // timer events
#include "strlib.h"
#include "grid.h"    // for the grid object
#include "random.h"  // for random number generation
#include "gwindow.h" // for animation controls

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay


static void welcome();

int fillCell();

void createCopy(const Grid<int>& grid, Grid<int>& gridCopy);

void initialize(Grid<int>& grid);

int setSpeed();

void printBoard(LifeDisplay& display, const Grid<int>& grid, Grid<int>& gridCopy, const int rows, const int cols);

int countNeighbors(Grid<int>& grid, const int row, const int col);

int nextGeneration(Grid<int>& grid, int neighbors, int row, int col);

void buildGridFromFile(Grid<int>& grid);

int setGridElement(string line, unsigned int index);

void setNextGeneration(const Grid<int>& grid, Grid<int>& gridCopy, const int rows, const int cols);

/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");
    Grid<int> board;
    Grid<int> boardCopy;
    welcome();
    initialize(board);
    createCopy(board, boardCopy);
    int speed = setSpeed();
    int rows = board.numRows();
    int cols = board.numCols();

    cout << getLine("Press [enter] to start simulation.") << endl;

    while(true) {

        printBoard(display, board, boardCopy, rows, cols);

        if (board.equals(boardCopy)) {
            break;
        }

        board = boardCopy;

        pause(speed);
    }

    return 0;
}

/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}

/**
  * Function: fillCell
  * ------------------
  * Sets the value of grid cell
  */
int fillCell() {
    if(randomInteger(0,1) == 0){
        return 0;
    }
    else {
        return randomInteger(1, kMaxAge);
    }
}

/**
  * Function: createCopy
  * -------------------
  * Make copy of your grid
  */
void createCopy(const Grid<int>& grid, Grid<int>& gridCopy) {
    int rows = grid.numRows();
    int cols = grid.numCols();

    gridCopy.resize(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            gridCopy.set(i,j,grid.get(i,j));
        }
    }
}

/**
  * Function: initialize
  * --------------------
  * Creates a new Grid object based on user input.
  */
void initialize(Grid<int>& grid) {
    cout << "Do you want to start with a random grid or upload your own?" << endl;
    cout << "\t1. Press 1 for randomly generated." << endl;
    cout << "\t2. Press 2 to upload your own." << endl;

    int startChoice = getIntegerBetween("Press 1 or 2then [enter]:", 1, 2);

    if (startChoice == 1) {
        grid.resize(randomInteger(40,60), randomInteger(40,60));
        for(int r = 0; r < grid.numRows(); ++r) {
            for(int c = 0; c < grid.numCols(); ++c) {
                grid.set(r,c,fillCell());
            }
        }
    }
    else if (startChoice == 2) {
        buildGridFromFile(grid);
    }
}

/**
  * Function: setSpeed
  * ------------------
  * Set the speed of the simulation based on user input
  */
int setSpeed() {
    cout << "Set simulation speed:" << endl;
    cout << "\t1. Fast" << endl;
    cout << "\t2. Medium" << endl;
    cout << "\t3. Slow" << endl;
    cout << "\t4. Manual input" << endl;

    int speedChoice = getIntegerBetween("Make selection then press [enter]: ", 1,4);

    switch(speedChoice) {
        case 1:
                speedChoice = 500;
                break;
        case 2:
                speedChoice = 1000;
                break;
        case 3:
                speedChoice = 2000;
                break;
        case 4:
                speedChoice = getInteger("Enter time(ms): ");
                break;
    }

    return speedChoice;
}

/**
 * Function: printBoard
 * --------------------
 * Displays board on screen
 */
void printBoard(LifeDisplay& display, const Grid<int>& grid, Grid<int>& gridCopy, const int rows, const int cols) {

    display.setDimensions(rows, cols);

    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            display.drawCellAt(i, j, grid.get(i,j));
        }
    }

    display.repaint();

    setNextGeneration(grid, gridCopy, rows, cols);

    // display.printBoard();
}

/**
  * Function: countNeighbors
  * ------------------------
  * count number of element's neighbors
  */
int countNeighbors(const Grid<int>& grid, const int row, const int col) {
    int neighbors = 0;
    for (int i = row-1; i < row+2; ++i) {
        for (int j = col-1; j < col+2; ++j) {
            if (grid.inBounds(i, j)) {
                if (grid.get(i,j) > 0) {
                    if (i == row && j == col) {
                        neighbors += 0;
                    }
                    else {
                        ++neighbors;
                    }
                }
            }
        }
    }
    return neighbors;
}

/**
 * @brief nextGeneration
 * @param grid
 * @param neighbors
 * @param row
 * @param col
 * @return
 * ----------------------
 * returns value of cell's next generation
 */
int nextGeneration(Grid<int>& grid, int neighbors, int row, int col) {
    int next;
    switch(neighbors) {
        case 2:
                next = grid.get(row, col);
                break;
        case 3:
                next = (grid.get(row,col) +1);
                break;
        default:
                next = 0;
                break;
    }
    return next;
}

/**
 * @brief buildGridFromFile
 * @param grid
 * Creates a grid based on a file
 */
void buildGridFromFile(Grid<int>& grid) {
    ifstream input;
    input.open("Colony.txt");
    if (input.fail()) {
        cout << "Error. Couldn't open file." << endl;
    }
    else {
        string line;
        int linePos = 0;
        int rows = 0;
        int cols = 0;
        while(getline(input, line)) {
            if (linePos <= 1) {
                switch(linePos) {
                    case 0:
                            rows = stringToInteger(line);
                            break;
                    case 1: cols = stringToInteger(line);
                            break;
                }
                cout << line << endl;
                if (linePos == 1) {
                    grid.resize(rows, cols);
                }
            }
            else {
                for(unsigned int i = 0; i < line.length(); ++i) {
                    cout << line[i];
                    grid.set(linePos-2, int(i), setGridElement(line, i));
                }
                cout << endl;
            }
            ++linePos;
        }
    }
    input.close();
}
/**
 * @brief setGridElement
 * @param line
 * @param index
 * @return
 * Sets grid's element value from file
 */
int setGridElement(string line, unsigned int index) {
    if (line[index] == 45) {
        return 0;
    }
    else {
        return randomInteger(1, kMaxAge);
    }
}

/**
 * @brief setNextGeneration
 * @param grid
 * @param gridCopy
 * @param rows
 * @param cols
 * Iterate through grid and set it's next generation value.
 */
void setNextGeneration(const Grid<int>& grid, Grid<int>& gridCopy, const int rows, const int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            gridCopy.set(i,j,nextGeneration(gridCopy, countNeighbors(grid, i, j), i, j));
        }
    }
}

