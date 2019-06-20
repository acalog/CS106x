/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim
#include "vector.h"
#include "map.h"
#include "stack.h"
#include "queue.h"

static const string kGrammarsDirectory = "grammars/";
static const string kGrammarFileExtension = ".g";

static string getNormalizedFilename(string filename);

static bool isValidGrammarFilename(string filename);

static string getFileName();

Vector<string> loadFile(string filename);

Map<string, Vector<string>> mapDefinitions(Vector<string> tempFile);

string getExpansion(Vector<string>& definition);

void vectorize(string& sentence, Vector<string>& list);

Stack<string> stackNonTerminals(string& start);

void queueNonTerminals(const string start, Queue<string>& nodes);

void printSentence(Vector<string>& sentence);

void replaceNonTerminal(Vector<string>& sentence, const string nonTerm, Vector<string>& tempSentence);

void expand(string& start, Vector<string>&  sentence, Map<string, Vector<string>>& directory);


int main() {
    while (true) {

        string filename = getFileName();
        if (filename.empty()) break;

        // Load grammar file
        Vector<string> templateFile = loadFile(getNormalizedFilename(filename));

        // Map grammar file in map of vectors (key:<nonterminal>, value: vector)
        Map<string, Vector<string>> directory = mapDefinitions(templateFile);

        // Expand <start> and save as string start
        for (int r = 0; r < 3; ++r) {
            string start = directory["<start>"].get(2);
            Vector<string> sentence;

            // Change start into vector of strings
            vectorize(start, sentence);

            expand(start, sentence, directory);

            printSentence(sentence);
        }

    }
    cout << "Thanks for playing!" << endl;
    return 0;
}

static string getNormalizedFilename(string filename) {
    string normalizedFileName = filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
        if (filename.empty() || isValidGrammarFilename(filename)) return filename;
        cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;
    }
}

Vector<string> loadFile(string filename) {
    ifstream infile;
    string line;
    Vector<string> templateFile;
    infile.open(filename.c_str());
    while (getline(infile, line)) {
        templateFile.add(line);
    }
    infile.close();
    return templateFile;
}

/*
 * Returns random expansion from definition
 */
string getExpansion(Vector<string>& definition) {
    int randomNumber = rand();
    int choice = ((randomNumber % stringToInteger(definition[1])));
    // Hotfix for definitions that erase contents
    if (2+choice >= definition.size())
        return definition[2+choice-1];
    return definition[2+choice];
}

/*
 * Return vector of strings built from single string.
 */
void vectorize(string& sentence, Vector<string>& list) {
    string word;
    bool nontermFound = false;
    for (unsigned int w = 0; w < sentence.length(); ++w) {
        if (sentence[w] == '<') {
            nontermFound = true;
            word += sentence[w];
        }
        else if (sentence[w] == '>') {
            word += sentence[w];
            list.add(word);
            nontermFound = false;
            word = "";
        }
        else if ((sentence[w] == ' ' && !nontermFound) || w == sentence.length()-1) {
            if (w == sentence.length()-1) {
                word += sentence[w];
            }
            if (word != "")
                list.add(word);
            word = "";
        }
        else {
            word += sentence[w];
        }
    }
}

// Create Stack collection of nonterminals
Stack<string> stackNonTerminals(string& start) {
    Stack<string> nodes;
    bool nontermFound = false;
    string nonterminal;
    for (unsigned int c = 0; c < start.length(); ++c) {
        if (start[c] == '<') {
            nonterminal += start[c];
            nontermFound = true;
        }
        else if (start[c] == ' ' && nontermFound) {
            nodes.push(nonterminal);
            nonterminal = "";
            nontermFound = false;
        }
        else if (start[c] == '>' && c == start.length()-1) {
            nonterminal += start[c];
            nodes.push(nonterminal);
            nonterminal = "";
            nontermFound = false;
        }
        else {
            if (nontermFound) {
                nonterminal += start[c];
            }
        }
    }
    return nodes;
}

Map<string, Vector<string>> mapDefinitions(Vector<string> tempFile) {
    Vector<string> definition;
    Map<string, Vector<string>> dir;
    for (int i = 0; i < tempFile.size(); ++i) {
        if (tempFile[i] == "" || i == tempFile.size()-1) {
            dir.put(definition[0], definition);
            definition.clear();
        }
        else {
            definition.add(tempFile[i]);
        }
    }
    return dir;
}

void queueNonTerminals(const string start, Queue<string>& nodes) {
    bool nontermFound = false;
    string nonterminal;
    for (unsigned int c = 0; c < start.length(); ++c) {
        if (start[c] == '<') {
            nonterminal += start[c];
            nontermFound = true;
        }
        /*
        else if ((start[c] == ' ' || start[c] == '.' || start[c] == ',') && nontermFound) {
            nodes.enqueue(nonterminal);
            nonterminal = "";
            nontermFound = false;
        }
        else if (start[c] == '>' && c == start.length()-1) {
            nonterminal += start[c];
            nodes.enqueue(nonterminal);
            nonterminal = "";
            nontermFound = false;
        }
        */
        else if (start[c] == '>') {
            nonterminal += start[c];
            nodes.enqueue(nonterminal);
            nonterminal = "";
            nontermFound = false;
        }
        else {
            if (nontermFound) {
                nonterminal += start[c];
            }
        }
    }
}

void printSentence(Vector<string>& sentence) {
    string displaySentence;
    for (int i = 0; i < sentence.size(); ++i) {
        displaySentence += sentence[i] + " ";
    }
    cout << displaySentence << endl;
    cout << endl;
}

void replaceNonTerminal(Vector<string>& sentence, const string nonTerm, Vector<string>& tempSentence) {
    for (int t = 0; t < sentence.size(); ++t) {
        if (sentence[t] == nonTerm) {
            Vector<string> start = sentence.subList(0,t);
            Vector<string> end = sentence.subList(t+1, ((sentence.size()-1)-t));
            sentence = start + tempSentence + end;
        }
    }
}

void expand(string& start, Vector<string>&  sentence, Map<string, Vector<string>>& directory) {
    Queue<string> Qnodes;
    queueNonTerminals(start, Qnodes);

    while (!Qnodes.isEmpty()) {
        string root = Qnodes.dequeue();
        Vector<string> def = directory[root];
        string nonTerm;
        string expansion;
        if (!def.isEmpty()) {
            nonTerm = def[0];
            expansion = getExpansion(def);
        }

        Vector<string> tempSentence;
        vectorize(expansion, tempSentence);

        replaceNonTerminal(sentence, nonTerm, tempSentence);

        queueNonTerminals(expansion, Qnodes);
    }
}

