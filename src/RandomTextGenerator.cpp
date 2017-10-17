// This program prints out N-grams given a text file: a user can generate a custom number of "random" words based on their tendencies to follow N
// other consecutive words in said text. Each generated random word is then based on the previously generated N random words to make seemingly
// sensible sentences.

#include <iostream>
#include "console.h"
#include <fstream>
#include "simpio.h"
#include "strlib.h"
#include "filelib.h"
#include "map.h"
#include "hashset.h"
#include "random.h"
using namespace std;

// This function constructs a map relating n words to a vector of possible words that follows it. It takes in a reference to an empty Map,
// a Vector of strings that contains the text file, and an int N (number of previous words for each random word to be based on).
void nGram(Map <string,string> &gramList, Vector <string> fileText, int N) {
    for (int i = 0; i < fileText.size(); i++) {
        string keyWord = "";
        string newValue = "";
        for ( int j = 0; j < N - 1; j++) {
            if (j == 0) {
                keyWord = fileText.get(i);
            } else {
                keyWord += " " + fileText.get((i + j ) % (fileText.size()));
            }
        }
        newValue = fileText.get( (i + N - 1) % (fileText.size()));
        if (gramList.containsKey(keyWord)) {
            newValue = gramList.get(keyWord) + " " + newValue;
            gramList.put(keyWord,newValue);
        } else {
            gramList.put(keyWord,newValue);
        }
    }
    return;
}

//This function takes in a string of words, separated by a space, and returns a single word out of the string by random.
string nextWord(string values) {
    Vector <int> wordStartIndexList;
    wordStartIndexList.add(0);
    for (int h = 0; h < values.size(); h++) {
        if (values[h]==' ') {
            wordStartIndexList.add(h+1);
        }
    }
    int randomWeight = randomInteger(0,wordStartIndexList.size()-1);
    wordStartIndexList.add(values.size());
    int wordStartIndex = wordStartIndexList[randomWeight];
    int wordEndIndex = wordStartIndexList[randomWeight+1];
    string generatedWord = "";
    for (int i = wordStartIndex; i < wordEndIndex; i++) {
        if (values[i] != ' ') {
            generatedWord+= values[i];
        }
    }
    return generatedWord;
}

// This function prompts the user to generate a number of random words using the N-gram algorithm, and calculates/outputs
// that number of words. It takes in a Map of keys and values to generate the words, and an integer N (number of previous words for each random word to be based on).
void generate(Map <string,string> &gramList, int N) {
    while (true) {
        Vector <string> allKeys = gramList.keys();
        string output = "";
        int wordGenNum = getInteger("# of random words to generate (0 to quit): ", "Illegal integer format. Try again.");
        if (wordGenNum == 0)
        {
            return;
        } else if (wordGenNum < N) {
            cout << "Must be at least " << N << " words." << endl;
        } else {
            // This generates a random key to start and makes sure that it begins with a capital letter.
            int randomNum = randomInteger(0, allKeys.size() - 1);
            string randWord = allKeys.get(randomNum);
            output = randWord;
            int stopPoint = (wordGenNum - N + 1);
            for (int i = 0; i < stopPoint; i++) {
                string nextKey = "";
                int spaceCount = 0;
                // This finds the next key to use by taking out the last two words of the output string.
                for (int j = output.size() - 1; j >= 0; j--) {
                    if (j == 0) {
                        nextKey = output;
                        break;
                    }
                    if (output[j] == ' ') {
                        spaceCount++;
                    }
                    if (spaceCount == (N-1) ){
                        nextKey = output.substr(j+1);
                        break;
                    }
                }
                string addedWord = nextWord(gramList.get(nextKey));
                output+= " "+ addedWord;
            }
            cout << "... " << output << " ..." << endl;
            cout << endl;
        }
    }
}


int main() {
    bool loop = true;
    while (loop) {
        cout << "Welcome to random text generator!" << endl;
        cout << "This program generates random text based on a document." << endl;
        cout << "Give me an input file and an 'N' value for groups of" << endl;
        cout << "words to base the algorithm on and I will generate random text for you."<< endl;
        cout <<  "I recommend N<4 for lolz or 8<N<15 if you want to see stuff that kinda makes sense." << endl;
        cout << "Type a number to pick from one of the options below, or enter your own .txt file name." << "\n" << endl;
        cout << "1 - Hamlet" << endl;
        cout << "2 - Moby Dick" << endl;
        cout << "3 - Romeo and Juliet" << endl;
        cout << "4 - Horror stories from Reddit" << endl;
        cout << "5 - General horror from my Organic Chemistry Textbook" << endl;
        cout << "6 - The US Constitution" << "\n" << endl;
        bool exists;
        Vector <string> fileText;

        do {
            string fileName = getLine("Input file name: ");
            switch(stoi(fileName)) {
                case 1: fileName = "hamlet.txt";
                        break;
                case 2: fileName = "mobydick.txt";
                        break;
                case 3: fileName = "romeoandjuliet.txt";
                        break;
                case 4: fileName = "horrorstory.txt";
                        break;
                case 5: fileName = "Orgo.txt";
                        break;
                case 6: fileName = "constitution.txt";
                        break;
            }
            exists = fileExists(fileName);
            if(!exists) {
                cout << "Unable to open that file.  Try again." << endl;
            } else {
                ifstream inputfile;
                inputfile.open(fileName);
                string word;
                while (!inputfile.eof()) {
                    inputfile >> word;
                    fileText.add(word);
                }
            }
        } while (!exists);
        int N = 0;
        int totalWords = fileText.size();
        while ((N < 2) || (N > totalWords)) {
            N = getInteger("Value of N: ", "Illegal integer format. Try again.");
            if (N < 2) {
                cout << "N must be 2 or greater." << endl;
            } else if (N > totalWords) {
                cout << "N must be smaller than the total number of words in the document: " << totalWords << "." << endl;
            }
        }
        Map <string, string> gramList;
        nGram(gramList, fileText, N);
        generate(gramList, N);
        loop = getYesOrNo("Would you like to generate from another file? (Y/N)");
    }
    cout << "Exiting." << endl;
    return 0;
}
