#include <iostream>
#include <fstream>
#include <string>
#include "quadList.hpp"

using std::string;
using std::getline;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;

int main()
{

/*
    LinkedList myList;

    cout << "printing empty list"<< endl;
    myList.print();
    cout << "-------------------------------------------"<< endl;
    cout << "removing from empty list"<< endl;
    myList.removeFrontLink();
    myList.print();
    cout << "-------------------------------------------"<< endl;
    cout << "adding one character"<< endl;
    myList.addLink('a');
    myList.print();
    cout << "-------------------------------------------"<< endl;
    cout << "removing character"<< endl;
    myList.removeFrontLink();
    myList.print();
    cout << "-------------------------------------------"<< endl;
    cout << "remove character again"<< endl;
    myList.removeFrontLink();
    myList.print();
    cout << "-------------------------------------------"<< endl;
    cout << "add 5 characters" << endl;
    myList.addLink('a');
    myList.addLink('a');
    myList.addLink('b');
    myList.addLink('c');
    myList.addLink('d');
    myList.print();
    cout << "-------------------------------------------"<< endl;
    cout << "removing all characters" << endl;
    myList.clearList();
    myList.print();
*/

    int         i,
                twoLetterCount = 0,
                threeLetterCount = 0;
    char        curChar;
    string      curString;
    ifstream    inputFile;
    QuadList    qList;

    inputFile.open("p2Input");

    while (getline(inputFile, curString))
    {
        i = 0;
        curChar = curString[i];
        //cout << "current string: " << curString << "      start char: " << curChar << endl;
        while (curChar != '\0')
        {
            // cout << "cur char: " << curChar << endl;
            qList.addChar(curChar);
            i++;
            curChar = curString[i];
        }
        //qList.print();
        if (qList.getTwoSize() > 0)
            twoLetterCount++;
        if (qList.getThreeSize() > 0)
            threeLetterCount++;
        //cout << "before clear: " << endl;
        //qList.print();
        qList.clearList();
        //cout << "after clear: " << endl;
        //qList.print();
    }

    cout << "Strings with letters that appear exactly twice: " << twoLetterCount << endl;
    cout << "Strings with letters that appear exactly 3 times: " << threeLetterCount << endl;
    cout << "Multiplying them together: " << twoLetterCount * threeLetterCount << endl;

    inputFile.close();

    return 0;
}
