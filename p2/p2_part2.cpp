#include <iostream>
#include <fstream>
#include <string>
#include "stringList.hpp"

using std::string;
using std::getline;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;

int main()
{
    int         i,
                bestDifference = -1,
                iterReturn = 0;
    string      curString,
                curStringBestMatch;
    ifstream    inputFile;
    LinkedList  stringList;


    inputFile.open("p2Input");

    // add all strings to linked list
    while (getline(inputFile, curString))
        stringList.addLink(curString);

    // find strings that are only different by one letter
    while ((bestDifference == -1 || bestDifference != 1) && iterReturn != -2)
    {
        iterReturn = stringList.iterNext();
        curString = stringList.iterGetString();
        bestDifference = stringList.getBestMatch(stringList.iterGetStringPtr(), &curStringBestMatch);
    }
    // print out results
    cout << "The strings are: " << curString << " and " << curStringBestMatch << endl;
    cout << "The common letters are: ";
    for (i = 0; i < curString.length(); i++)
    {
        if (curString[i] == curStringBestMatch[i])
            cout << curString[i];
    }
    cout << endl;

    inputFile.close();

    return 0;
}
