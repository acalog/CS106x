/**
 * File: boggle.cpp
 * ----------------
 * Implements the game of Boggle.
 */

#include <cctype>
#include <iostream>
#include <ctype.h>
#include "console.h"
#include "gboggle.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "random.h"
#include "grid.h"
#include "map.h"
#include "set.h"
#include "lexicon.h"
#include "cube.h"
using namespace std;

static const string kStandardCubes[16] = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

static const string kBigBoggleCubes[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

static const int kMinLength = 4;
static const double kDelayBetweenHighlights = 100;
static const double kDelayAfterAllHighlights = 500;

static const string kEnglishLanguageDatafile = "dictionary.txt";


static void welcome();

static void giveInstructions();

static int getPreferredBoardSize();

Vector<string> copyBoggleBoard(const int dimension);

static void playBoggle();

void shuffleBoggleBoard(Vector<string>& vec);

void printBoard(const Vector<string> vec);

void drawShuffleBoard(Grid<char>& loadGrid);

Grid<char> loadGrid(const Vector<string> vec, const int dimension);

Grid<char> createBoggleGrid(const int dimension);

void findAllAnswers(const Lexicon& english, Grid<char>& bog, Set<string>& bank, Map<string, Vector<block>>& answerMap);

void traceWord(Grid<char>& bog, int row, int col, string& word, Grid<bool>& visited, const Lexicon& english, Set<string>& bank, Map<string, Vector<block>>& answerMap, Vector<block>& wordMap);

static string getWord(const string& prompt);

int checkAnswer(Map<string, Vector<block>>& answerMap, const string word);

/**
 * Function: main
 * --------------
 * Serves as the entry point to the entire program.
 */
int main() {
    GWindow gw(kBoggleWindowWidth, kBoggleWindowHeight);
    initGBoggle(gw);
    welcome();
    if (getYesOrNo("Do you need instructions?")) giveInstructions();

    do {
        playBoggle();
    } while (getYesOrNo("Would you like to play again?"));

    cout << "Thank you for playing!" << endl;
    shutdownGBoggle();
    return 0;
}

/**
 * Function: playBoggle
 * --------------------
 * Manages all details needed for the user to play one
 * or more games of Boggle.
 */
static void playBoggle() {
    int dimension = getPreferredBoardSize();
    Lexicon english(kEnglishLanguageDatafile);
    // Initialize a blank board.
    drawBoard(dimension, dimension);

    Grid<char> boggleGrid = createBoggleGrid(dimension);

    drawShuffleBoard(boggleGrid);

    Set<string> bank;
    Map<string, Vector<block>> answerMap;
    findAllAnswers(english, boggleGrid, bank, answerMap);
    int score = 0;
    while (true) {
        string word = getWord("Answers: (Press enter to quit) ");
        if (word == "")
            break;
        // word = toUpperCase(word);
        score += checkAnswer(answerMap, toUpperCase(word));
    }
    cout << endl << "Player's score: " << score << endl;
}

/**
 * Function: checkAnswer
 * Check if player's word is in answer bank,
 * Highlight on board if yes and return 1 point.
 */
int checkAnswer(Map<string, Vector<block>>& answerMap, const string word) {
    if (answerMap.containsKey(word)) {
        Vector<block> wordMap = answerMap[word];
        for (int i = 0; i < wordMap.size(); ++i) {
            highlightCube(wordMap[i].row, wordMap[i].col, true);
        }
        pause(10000);
        for (int i = 0; i < wordMap.size(); ++i) {
            highlightCube(wordMap[i].row, wordMap[i].col, false);
        }
        return 1;
    }
    else {
        return 0;
    }
}

/**
 * Function: getPreferredBoardSize
 * -------------------------------
 * Repeatedly prompts the user until he or she responds with one
 * of the two supported Boggle board dimensions.
 */
static int getPreferredBoardSize() {
    cout << "You can choose standard Boggle (4x4 grid) or Big Boggle (5x5 grid)." << endl;
    return getIntegerBetween("Which dimension would you prefer: 4 or 5?", 4, 5);
}

/**
 * Function: copyBoggleBoard
 * -------------------------
 * Copies const string array
 * into Vector<string> so it
 * can be manipulated
 */
Vector<string> copyBoggleBoard(const int dimension) {
    Vector<string> vec;
    if (dimension == 4) {
        for (int i = 0; i < 16; ++i) {
            vec.add(kStandardCubes[i]);
        }
    }
    else {
        for (int i = 0; i < 25; ++i) {
            vec.add(kBigBoggleCubes[i]);
        }
    }

    return vec;
}

/**
 * Function: shuffleBoggleBoard
 * ----------------------------
 * Randomly shuffles positions
 * of strings within boggle board
 */
void shuffleBoggleBoard(Vector<string>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        int r = randomInteger(i,vec.size()-1);
        string temp = vec.get(i);
        vec.set(i, vec.get(r));
        vec.set(r, temp);
    }
}

/**
 * Function: printBoard
 * --------------------
 * Print out vector representation of
 * game board to screen
 */
void printBoard(const Vector<string> vec) {
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << endl;
    }
}

/**
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */
static void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl;
    cout << endl;
}

/**
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */
static void giveInstructions() {
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl;
    cout << endl;
    cout << "Hit return when you're ready...";
    getLine(); // ignore return value
}

/**
 * Function: loadGrid
 * ------------------
 * Create a grid object representing
 * a shuffled boggle board.
 */
Grid<char> loadGrid(const Vector<string> vec, const int dimension) {
    Grid<char> boggleGrid(dimension, dimension);
    int d = 0;
    for (int r = 0; r < dimension; ++r) {
        for (int c = 0; c < dimension; ++c) {
            // unsigned int dice = randomInteger(0,5);
            unsigned int dice = rand() % 5;
            string block = vec[d];
            boggleGrid.set(r,c,block.at(dice));
            labelCube(r,c,block.at(dice));
            ++d;
        }
    }
    return boggleGrid;
}

/**
 * Function: drawShuffleBoard
 * --------------------------
 * Print board to graphical output from Grid.
 */
void drawShuffleBoard(Grid<char>& loadGrid) {
    for (int r = 0; r < loadGrid.numRows(); ++r) {
        for (int c = 0; c < loadGrid.numCols(); ++c) {
            labelCube(r,c,loadGrid.get(r,c));
        }
    }
}

/**
 * Function: createBoggleGrid
 * --------------------------
 * Creates a grid representation of gameboard
 * based on dimension input by user
 */
Grid<char> createBoggleGrid(const int dimension) {
    // Create vector object to represent boggle blocks.
    Vector<string> vec = copyBoggleBoard(dimension);

    // Shuffle the blocks and roll the blocks for the gameboard.
    shuffleBoggleBoard(vec);

    // Create Grid object representing boggle gameboard.
    return loadGrid(vec, dimension);
}

/**
 * Function: findAllAnswers
 * ------------------------
 * Find all possible solutions to boggle board
 * and save in a vector or word-bank.
 */
void findAllAnswers(const Lexicon& english, Grid<char>& bog, Set<string>& bank, Map<string, Vector<block>>& answerMap) {
    // Starting at [0,0], look for all possible word built of that letter
    int nRows = bog.numRows();
    int nCols = bog.numCols();
    Grid<bool> visited(nRows, nCols, false);
    string word;
    Vector<block> wordMap;

    for (int r = 0; r < nRows; ++r) {
        for (int c = 0; c < nCols; ++c) {
            traceWord(bog, r, c, word, visited, english, bank, answerMap, wordMap);
        }
    }
    cout << bank.toString() << endl;
}

/**
 * Function: traceWord
 * Build words based on the letters of the boggle board
 */
void traceWord(Grid<char>& bog, int row, int col, string& word, Grid<bool>& visited, const Lexicon& english, Set<string>& bank, Map<string, Vector<block>>& answerMap, Vector<block>& wordMap) {
    word += bog.get(row,col);
    block b;
    b.row = row;
    b.col = col;
    b.letter = bog.get(row,col);
    wordMap.add(b);
    visited.set(row,col,true);
    if (word.length() >= 4 && english.contains(word) && !bank.contains(word)) {
        bank.add(word);
        answerMap.put(word, wordMap);
    }
    for (int a = row-1; a < row+2; ++a) {
        for (int b = col-1; b < col+2; ++b) {
            if (bog.inBounds(a,b) && english.containsPrefix(word)) {
                if (!visited.get(a,b)) {
                    traceWord(bog, a, b, word, visited, english, bank, answerMap, wordMap);
                }
            }
        }
    }
    word.erase(word.length()-1);
    visited.set(row,col,false);
    wordMap.remove(wordMap.size()-1);
}

/**
 * Function: getWord
 * Get words from user to check against the boggle board.
 */
static string getWord(const string& prompt) {
    while (true) {
        string response = trim(getLine(prompt));
        return response;
    }
}

