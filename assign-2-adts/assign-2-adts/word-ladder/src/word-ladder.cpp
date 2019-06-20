/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "vector.h"
#include "queue.h"
#include "stack.h"

void cycleWords(const Lexicon& english, const string& start);

void printVect(const Vector<string>& vec);

static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "\"." << endl;
    // Queue<Stack <string>> Q;
    Queue<Vector <string>> Q;
    // Stack<string> ladder;
    Vector<string> ladder;
    Vector<string> wordTracker;
    Vector<string> partial;
    ladder.add(start);
    wordTracker.add(start);
    Q.enqueue(ladder);

    while (!Q.isEmpty()) {
        ladder = Q.dequeue();
        if (ladder.get(ladder.size()-1) == end) {
            cout << "Ladder: ";
            for (int l = 0; l < ladder.size(); ++l) {
                cout << ladder[l] << " ";
            }
            cout << endl;
            break;
        }
        else {
            string root = ladder.get(ladder.size()-1);
            string newStr = root;
            for (unsigned int i = 0; i < root.length(); ++i) {
                for (int j = 0; j <= 26; ++j) {
                    newStr[i] = (root[i] - 97 + j) % 26 + 97;
                    if (english.contains(newStr) && !wordTracker.contains(newStr)) {
                        // Create copy of partial ladder
                        partial = ladder;
                        // Push new word onto top of partial ladder copy
                        partial.add(newStr);
                        wordTracker.add(newStr);
                        // cout << "Ladder: " << partial << endl;
                        // Enqueue copy at end of queue
                        Q.enqueue(partial);
                        // cout << "Queue: " << Q << endl;
                        // cout << "Found " << newStr << " in dictionary!" << endl;
                    }
                }
            }
        }
    }
}

static const string kEnglishLanguageDatafile = "dictionary.txt";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}

void printVect(const Vector<string>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

void cycleWords(const Lexicon& english, const string& start) {
    string compare = start;
    for (unsigned int i = 0; i < start.length(); ++i) {
        for (int j = 0; j < 26; ++j) {
            compare[i] = (start[i] - 97 + j) % 26 + 97;
            if (english.contains(compare)) {
                cout << "Found " << compare << " in dictionary!" << endl;
            }
        }
    }
}
